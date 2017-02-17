#include "applicationtasks.h"
#include "calliperutil/global/globalinstance.h"
#include "model/stores/resourceenvironment.h"
#include "model/shaders/simplelitshader.h"

namespace
{
    typedef CalliperUtil::GlobalInstance<Model::ResourceEnvironment> ResourceEnvironmentInstance;

    ResourceEnvironmentInstance* g_pResourceEnv;

    void loadFailsafeShaders()
    {
        CalliperUtil::GlobalInstance<Model::ResourceEnvironment>& resourceEnv = *g_pResourceEnv;
        Model::ShaderStore* shaderStore = resourceEnv->shaderStore();

        quint16 failsafeShader = shaderStore->addShaderProgram<Model::SimpleLitShader>();
        Model::ShaderPalette palette;
        palette.addItem(Model::ShaderPalette::DefaultShader, failsafeShader);

        resourceEnv->setFailsafeShaderPalette(palette);
    }

    void loadFailsafeTextures()
    {
        CalliperUtil::GlobalInstance<Model::ResourceEnvironment>& resourceEnv = *g_pResourceEnv;
        Model::MaterialResourceManager* resourceManager = resourceEnv->materialResourceManager();

        resourceManager->textureStore()->setDefaultTextureFromFile(":model/textures/_ERROR_");
    }
}

namespace AppCalliper
{
    namespace ApplicationTasks
    {
        void initSubSystems()
        {
            g_pResourceEnv = new ResourceEnvironmentInstance();

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
