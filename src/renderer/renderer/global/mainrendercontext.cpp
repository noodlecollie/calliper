#include "mainrendercontext.h"
#include <QGuiApplication>

namespace Renderer
{
    Q_LOGGING_CATEGORY(lcMainRenderContext, "Renderer.MainRenderContext")

    MainRenderContext* MainRenderContext::m_pGlobalInstance = Q_NULLPTR;

    void MainRenderContext::globalInitialise()
    {
        Q_ASSERT_X(!m_pGlobalInstance, Q_FUNC_INFO, "Global instance already initialised!");
        if ( m_pGlobalInstance )
        {
            return;
        }

        m_pGlobalInstance = new MainRenderContext();
    }

    void MainRenderContext::globalShutdown()
    {
        Q_ASSERT_X(m_pGlobalInstance, Q_FUNC_INFO, "Global instance not initialised!");

        delete m_pGlobalInstance;
        m_pGlobalInstance = Q_NULLPTR;
    }

    MainRenderContext* MainRenderContext::globalInstance()
    {
        return m_pGlobalInstance;
    }

    MainRenderContext::MainRenderContext()
        : m_pSurface(new QOffscreenSurface()),
          m_pContext(new QOpenGLContext()),
          m_bCreatedSuccessfully(false)
    {
        QSurfaceFormat expectedFormat = QSurfaceFormat::defaultFormat();
        m_pSurface->setFormat(expectedFormat);
        m_pSurface->create();

        QSurfaceFormat actualFormat = m_pSurface->format();

        if ( actualFormat != expectedFormat )
        {
            qCWarning(lcMainRenderContext) << "Expected OpenGL format of" << expectedFormat
                                                 << "but instead got format of" << actualFormat;
        }

        m_pContext->setScreen(QGuiApplication::screens().at(0));
        m_pContext->setShareContext(QOpenGLContext::globalShareContext());
    }

    MainRenderContext::~MainRenderContext()
    {
        if ( QOpenGLContext::currentContext() == m_pContext )
        {
            m_pContext->doneCurrent();
        }

        delete m_pContext;

        m_pSurface->destroy();
        delete m_pSurface;
    }

    bool MainRenderContext::create()
    {
        if ( m_bCreatedSuccessfully )
        {
            return true;
        }

        m_bCreatedSuccessfully = m_pContext->create();
        return m_bCreatedSuccessfully;
    }

    bool MainRenderContext::isCreated() const
    {
        return m_bCreatedSuccessfully;
    }

    bool MainRenderContext::makeCurrent()
    {
        return m_pContext->makeCurrent(m_pSurface);
    }

    void MainRenderContext::doneCurrent()
    {
        m_pContext->doneCurrent();
    }

    QOpenGLContext* MainRenderContext::context() const
    {
        return m_pContext;
    }

    QOffscreenSurface* MainRenderContext::surface() const
    {
        return m_pSurface;
    }
}
