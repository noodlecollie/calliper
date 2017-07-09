#include "rendersystem.h"

#include <QOpenGLContext>
#include <QOffscreenSurface>

#include "rendersystem/private/static-stores/openglshaderstore/openglshaderstore.h"
#include "rendersystem/private/static-stores/rendermodestore/rendermodestore.h"

#include "rendersystem/private/stores/opengltexturestore/opengltexturestore.h"
#include "rendersystem/private/stores/materialstore/materialstore.h"
#include "rendersystem/private/stores/rendermodelstore/rendermodelstore.h"
#include "rendersystem/private/stores/framebufferstore.h/framebufferstore.h"

namespace
{
    QOpenGLContext* g_pMainContext = Q_NULLPTR;
    QOffscreenSurface* g_pOffscreenSurface = Q_NULLPTR;
    bool g_bInitialised = false;

    inline bool makeCurrentInternal()
    {
        return g_pMainContext->makeCurrent(g_pOffscreenSurface);
    }

    inline void doneCurrentInternal()
    {
        g_pMainContext->doneCurrent();
    }

    template<typename T>
    void initialiseStore()
    {
        T::globalInitialise();
    }

    template<typename T>
    void shutdownStore()
    {
        T::globalShutdown();
    }

    template<typename T>
    void initialiseStaticStore()
    {
        initialiseStore<T>();
        T::globalInstance()->initialise();
    }

    template<typename T>
    void shutdownStaticStore()
    {
        T::globalInstance()->destroy();
        shutdownStore<T>();
    }
}

void initialiseStores()
{
    initialiseStore<FrameBufferStore>();

    initialiseStaticStore<OpenGLShaderStore>();
    initialiseStaticStore<RenderModeStore>();

    initialiseStore<OpenGLTextureStore>();
    initialiseStore<MaterialStore>();
    initialiseStore<RenderModelStore>();
}

void shutdownStores()
{
    shutdownStore<RenderModelStore>();
    shutdownStore<MaterialStore>();
    shutdownStore<OpenGLTextureStore>();

    shutdownStaticStore<RenderModeStore>();
    shutdownStaticStore<OpenGLShaderStore>();

    shutdownStore<FrameBufferStore>();
}

namespace RenderSystem
{
    namespace Global
    {
        bool initialise()
        {
            if ( g_bInitialised )
            {
                Q_ASSERT_X(false, Q_FUNC_INFO, "Render system already initialised!");
                return false;
            }

            g_pMainContext = new QOpenGLContext();
            g_pMainContext->setFormat(QSurfaceFormat::defaultFormat());

            if ( !g_pMainContext->create() )
            {
                Q_ASSERT_X(false, Q_FUNC_INFO, "Unable to create main context!");
                return false;
            }

            g_pOffscreenSurface = new QOffscreenSurface();
            g_pOffscreenSurface->setFormat(QSurfaceFormat::defaultFormat());
            g_pOffscreenSurface->create();

            if ( !g_pOffscreenSurface->isValid() )
            {
                Q_ASSERT_X(false, Q_FUNC_INFO, "Main offscreen surface not valid!");
                return false;
            }

            if ( !makeCurrentInternal() )
            {
                Q_ASSERT_X(false, Q_FUNC_INFO, "Unable to make main context current!");
                return false;
            }

            initialiseStores();
            doneCurrentInternal();

            g_bInitialised = true;
            return true;
        }

        void shutdown()
        {
            if ( !g_bInitialised )
            {
                Q_ASSERT_X(false, Q_FUNC_INFO, "Render system not initialised!");
                return;
            }

            if ( QOpenGLContext::currentContext() == g_pMainContext )
            {
                Q_ASSERT_X(false, Q_FUNC_INFO, "Context is current at point of shutdown - no context should be current here!");
            }

            bool current = makeCurrentInternal();
            if ( !current )
            {
                Q_ASSERT_X(false, Q_FUNC_INFO, "Could not make context current!");
            }

            shutdownStores();
            doneCurrentInternal();

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
