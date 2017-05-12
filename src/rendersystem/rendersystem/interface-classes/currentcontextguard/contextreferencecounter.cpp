#include "contextreferencecounter.h"

#include <QOpenGLContext>

#include "rendersystem/global/rendersystem.h"

namespace RenderSystem
{
    Q_LOGGING_CATEGORY(lcContextReferenceCounter, "RenderSystem.ContextReferenceCounter")

    quint32 ContextReferenceCounter::m_nRefCount = 0;
    QOpenGLContext* ContextReferenceCounter::m_pPreviouslyCurrentContext = Q_NULLPTR;
    QSurface* ContextReferenceCounter::m_pPreviouslyCurrentSurface = Q_NULLPTR;
    QOpenGLContext* ContextReferenceCounter::m_pOwnContext = Q_NULLPTR;
    bool ContextReferenceCounter::m_bMadeCurrent = false;

    ContextReferenceCounter::ContextReferenceCounter()
    {
        ++m_nRefCount;

        if ( m_nRefCount != 1 )
        {
            return;
        }

        qCDebug(lcContextReferenceCounter) << "Main render context being made current.";

        m_pPreviouslyCurrentContext = QOpenGLContext::currentContext();

        if ( m_pPreviouslyCurrentContext )
        {
            qCDebug(lcContextReferenceCounter) << "Previously current context being stored.";

            m_pPreviouslyCurrentSurface = m_pPreviouslyCurrentContext->surface();
            m_pPreviouslyCurrentContext->doneCurrent();
        }

        m_bMadeCurrent = Global::makeCurrent();

        if ( m_bMadeCurrent )
        {
            m_pOwnContext = QOpenGLContext::currentContext();
        }
    }

    ContextReferenceCounter::~ContextReferenceCounter()
    {
        Q_ASSERT_X(m_nRefCount > 0, Q_FUNC_INFO, "Reference counting out of sync!");
        if ( m_nRefCount == 0 )
        {
            return;
        }

        Q_ASSERT_X(!m_bMadeCurrent || QOpenGLContext::currentContext() == m_pOwnContext,
                   Q_FUNC_INFO,
                   "Other context made current instead of this one!");

        if ( m_bMadeCurrent && QOpenGLContext::currentContext() != m_pOwnContext )
        {
            qCWarning(lcContextReferenceCounter) << "Expecting main render context to be current, but a different context was current instead!";
        }

        --m_nRefCount;

        if ( m_nRefCount != 0 )
        {
            return;
        }

        if ( m_bMadeCurrent )
        {
            Global::doneCurrent();
            m_bMadeCurrent = false;
            m_pOwnContext = Q_NULLPTR;
        }

        if ( m_pPreviouslyCurrentContext )
        {
            qCDebug(lcContextReferenceCounter) << "Previously current context being restored.";

            m_pPreviouslyCurrentContext->makeCurrent(m_pPreviouslyCurrentSurface);
            m_pPreviouslyCurrentContext = Q_NULLPTR;
            m_pPreviouslyCurrentSurface = Q_NULLPTR;
        }
    }

    quint32 ContextReferenceCounter::refCount() const
    {
        return m_nRefCount;
    }
}
