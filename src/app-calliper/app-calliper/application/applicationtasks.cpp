#include "applicationtasks.h"
#include "model/global/resourceenvironment.h"
#include "model/shaders/simplelitshader.h"
#include "model/shaders/unlitpervertexcolorshader.h"
#include "renderer/global/mainrendercontext.h"
#include "calliperutil/debug/debug.h"
#include "renderer/opengl/scopedcurrentcontext.h"

namespace
{
    void loadFailsafeTextures()
    {
        Model::ResourceEnvironment* resourceEnv = Model::ResourceEnvironment::globalInstance();
        resourceEnv->textureStore()->setDefaultTextureFromFile(":model/textures/_ERROR_");
    }

    void shutDownOpenGLSystems()
    {
        Renderer::ScopedCurrentContext scopedContext;
        Q_UNUSED(scopedContext);

        Model::ResourceEnvironment::globalShutdown();
    }
}

namespace AppCalliper
{
    namespace ApplicationTasks
    {
        void initSubSystems()
        {
            Renderer::MainRenderContext::globalInitialise();
            CUTL_ASSERT_SUCCESS(Renderer::MainRenderContext::globalInstance()->create());

            Renderer::ScopedCurrentContext scopedContext;
            Q_UNUSED(scopedContext);

            Model::ResourceEnvironment::globalInitialise();
            loadFailsafeTextures();
        }

        void shutDownSubSystems()
        {
            shutDownOpenGLSystems();

            Renderer::MainRenderContext::globalShutdown();
        }
    }
}
