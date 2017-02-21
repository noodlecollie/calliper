#include "openglbackgroundcontext.h"
#include <QGuiApplication>

namespace Renderer
{
    OpenGLBackgroundContext* OpenGLBackgroundContext::m_pGlobalInstance = nullptr;

    void OpenGLBackgroundContext::globalInitialise()
    {
        Q_ASSERT_X(!m_pGlobalInstance, Q_FUNC_INFO, "Global instance already initialised!");
        if ( m_pGlobalInstance )
        {
            return;
        }

        m_pGlobalInstance = new OpenGLBackgroundContext();
    }

    void OpenGLBackgroundContext::globalShutdown()
    {
        Q_ASSERT_X(m_pGlobalInstance, Q_FUNC_INFO, "Global instance not initialised!");

        delete m_pGlobalInstance;
        m_pGlobalInstance = nullptr;
    }

    OpenGLBackgroundContext* OpenGLBackgroundContext::globalInstance()
    {
        return m_pGlobalInstance;
    }

    OpenGLBackgroundContext::OpenGLBackgroundContext()
        : m_pSurface(new QOffscreenSurface()),
          m_pContext(new QOpenGLContext()),
          m_bCreatedSuccessfully(false)
    {
        m_pSurface->setFormat(QSurfaceFormat::defaultFormat());
        m_pSurface->create();

        m_pContext->setScreen(QGuiApplication::screens().at(0));
        m_pContext->setShareContext(QOpenGLContext::globalShareContext());
    }

    OpenGLBackgroundContext::~OpenGLBackgroundContext()
    {
        if ( QOpenGLContext::currentContext() == m_pContext )
        {
            m_pContext->doneCurrent();
        }

        delete m_pContext;

        m_pSurface->destroy();
        delete m_pSurface;
    }

    bool OpenGLBackgroundContext::create()
    {
        if ( m_bCreatedSuccessfully )
        {
            return true;
        }

        m_bCreatedSuccessfully = m_pContext->create();
        return m_bCreatedSuccessfully;
    }

    bool OpenGLBackgroundContext::isCreated() const
    {
        return m_bCreatedSuccessfully;
    }

    bool OpenGLBackgroundContext::makeCurrent()
    {
        return m_pContext->makeCurrent(m_pSurface);
    }

    void OpenGLBackgroundContext::doneCurrent()
    {
        m_pContext->doneCurrent();
    }
}
