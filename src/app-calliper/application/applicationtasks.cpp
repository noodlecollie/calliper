#include "applicationtasks.h"
#include "calliperutil/global/globalinstance.h"
#include "model/global/resourceenvironment.h"
#include "model/shaders/simplelitshader.h"

namespace
{
    Model::ResourceEnvironmentInstance* g_pResourceEnv;

    void loadFailsafeShaders()
    {
        Model::ResourceEnvironmentInstance& resourceEnv = *g_pResourceEnv;
        Model::ShaderStore* shaderStore = resourceEnv->shaderStore();

        quint16 failsafeShader = shaderStore->addShaderProgram<Model::SimpleLitShader>();
        Model::ShaderPalette palette;
        palette.addItem(Model::ShaderPalette::DefaultShader, failsafeShader);

        resourceEnv->setFailsafeShaderPalette(palette);
    }

    void loadFailsafeTextures()
    {
        Model::ResourceEnvironmentInstance& resourceEnv = *g_pResourceEnv;
        resourceEnv->textureStore()->setDefaultTextureFromFile(":model/textures/_ERROR_");
    }
}

namespace AppCalliper
{
    namespace ApplicationTasks
    {
        void initSubSystems()
        {
            g_pResourceEnv = new Model::ResourceEnvironmentInstance();

            loadFailsafeShaders();
            loadFailsafeTextures();
        }

        void shutDownSubSystems()
        {
            delete g_pResourceEnv;
            g_pResourceEnv = nullptr;
        }
    }
}
