#include "scopedcurrentcontext.h"
#include <QOpenGLContext>
#include "renderer/global/mainrendercontext.h"

namespace Renderer
{
    QOpenGLContext* ScopedCurrentContext::m_pContext = Q_NULLPTR;

    ScopedCurrentContext::ScopedCurrentContext()
        : m_bInvalid(false),
          m_bShouldCleanUp(true)
    {
        MainRenderContext* renderContext = MainRenderContext::globalInstance();

        Q_ASSERT_X(!m_pContext || m_pContext == renderContext->context(), Q_FUNC_INFO, "ScopedCurrentContext already in use!");
        if ( m_pContext && m_pContext != renderContext->context() )
        {
            m_bInvalid = true;
            return;
        }

        if ( !m_pContext )
        {
            m_pContext = renderContext->context();
            m_pContext->makeCurrent(renderContext->surface());
        }
        else
        {
            m_bShouldCleanUp = false;
        }
    }

    ScopedCurrentContext::ScopedCurrentContext(QOpenGLContext *context, QSurface *surface)
        : m_bInvalid(false),
          m_bShouldCleanUp(true)
    {
        Q_ASSERT_X(!m_pContext, Q_FUNC_INFO, "ScopedCurrentContext is already in use!");
        if ( m_pContext )
        {
            m_bInvalid = true;
            return;
        }

        m_pContext = context;
        m_pContext->makeCurrent(surface);
    }

    ScopedCurrentContext::~ScopedCurrentContext()
    {
        if ( m_bInvalid || !m_bShouldCleanUp )
        {
            return;
        }

        m_pContext->doneCurrent();
        m_pContext = Q_NULLPTR;
    }

    bool ScopedCurrentContext::wasSuccessful() const
    {
        return !m_bInvalid;
    }
}
