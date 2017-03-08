#include "applicationtasks.h"
#include "model/global/resourceenvironment.h"
#include "model/shaders/simplelitshader.h"
#include "model/shaders/unlitpervertexcolorshader.h"
#include "renderer/global/openglbackgroundcontext.h"
#include "calliperutil/debug/debug.h"

namespace
{
    void loadFailsafeShaders()
    {
        Model::ResourceEnvironment* resourceEnv = Model::ResourceEnvironment::globalInstance();
        Model::ShaderPaletteStore* shaderPaletteStore = resourceEnv->shaderPaletteStore();

        Renderer::ShaderPalette palette;
        palette.addItem(Renderer::ShaderPalette::LitTextured, Model::KnownShaderDefs::SimpleLitShaderId);
        palette.addItem(Renderer::ShaderPalette::UnlitPerVertexColor, Model::KnownShaderDefs::UnlitPerVertexColourShaderId);

        shaderPaletteStore->setDefaultShaderPalette(palette);
    }

    void loadFailsafeTextures()
    {
        Model::ResourceEnvironment* resourceEnv = Model::ResourceEnvironment::globalInstance();
        resourceEnv->textureStore()->setDefaultTextureFromFile(":model/textures/_ERROR_");
    }
}

namespace AppCalliper
{
    namespace ApplicationTasks
    {
        void initSubSystems()
        {
            Renderer::OpenGLBackgroundContext::globalInitialise();
            CUTL_ASSERT_SUCCESS(Renderer::OpenGLBackgroundContext::globalInstance()->create());

            Model::ResourceEnvironment::globalInitialise();

            CUTL_ASSERT_SUCCESS(Renderer::OpenGLBackgroundContext::globalInstance()->makeCurrent());

            loadFailsafeShaders();
            loadFailsafeTextures();

            Renderer::OpenGLBackgroundContext::globalInstance()->doneCurrent();
        }

        void shutDownSubSystems()
        {
            CUTL_ASSERT_SUCCESS(Renderer::OpenGLBackgroundContext::globalInstance()->makeCurrent());

            Model::ResourceEnvironment::globalShutdown();

            Renderer::OpenGLBackgroundContext::globalInstance()->doneCurrent();

            Renderer::OpenGLBackgroundContext::globalShutdown();
        }
    }
}
