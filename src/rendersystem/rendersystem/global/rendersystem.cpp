#include "rendersystem.h"

#include <QOpenGLContext>
#include <QOffscreenSurface>

#include "rendersystem/private/stores/openglshaderstore/openglshaderstore.h"
#include "rendersystem/private/stores/opengltexturestore/opengltexturestore.h"
#include "rendersystem/private/stores/materialstore/materialstore.h"
#include "rendersystem/private/stores/rendermodestore/rendermodestore.h"

namespace
{
    QOpenGLContext* g_pMainContext = Q_NULLPTR;
    QOffscreenSurface* g_pOffscreenSurface = Q_NULLPTR;
    bool g_bInitialised = false;
}

void initialiseStores()
{
    OpenGLShaderStore::globalInitialise();
    RenderModeStore::globalInitialise();
    OpenGLTextureStore::globalInitialise();
    MaterialStore::globalInitialise();
}

void shutdownStores()
{
    MaterialStore::globalShutdown();
    OpenGLTextureStore::globalShutdown();
    RenderModeStore::globalShutdown();
    OpenGLShaderStore::globalShutdown();
}

namespace RenderSystem
{
    namespace Global
    {
        bool initialise()
        {
            Q_ASSERT_X(!g_bInitialised, Q_FUNC_INFO, "Render system already initialised!");
            if ( g_bInitialised )
            {
                return false;
            }

            g_pMainContext = new QOpenGLContext();
            g_pMainContext->setFormat(QSurfaceFormat::defaultFormat());

            if ( !g_pMainContext->create() )
            {
                return false;
            }

            g_pOffscreenSurface = new QOffscreenSurface();
            g_pOffscreenSurface->setFormat(QSurfaceFormat::defaultFormat());
            g_pOffscreenSurface->create();

            if ( !g_pOffscreenSurface->isValid() )
            {
                return false;
            }

            initialiseStores();

            g_bInitialised = true;
            return true;
        }

        void shutdown()
        {
            Q_ASSERT_X(g_bInitialised, Q_FUNC_INFO, "Render system not initialised!");
            if ( !g_bInitialised )
            {
                return;
            }

            shutdownStores();

            doneCurrent();

            g_pOffscreenSurface->destroy();
            delete g_pOffscreenSurface;
            g_pOffscreenSurface = Q_NULLPTR;

            delete g_pMainContext;
            g_pMainContext = Q_NULLPTR;

            g_bInitialised = false;
        }

        bool makeCurrent()
        {
            if ( !g_bInitialised || QOpenGLContext::currentContext() == g_pMainContext )
            {
                return false;
            }

            return g_pMainContext->makeCurrent(g_pOffscreenSurface);
        }

        void doneCurrent()
        {
            if ( !g_bInitialised || QOpenGLContext::currentContext() != g_pMainContext )
            {
                return;
            }

            g_pMainContext->doneCurrent();
        }
    }
}
