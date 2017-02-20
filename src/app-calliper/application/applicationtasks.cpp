#include "applicationtasks.h"
#include "model/global/resourceenvironment.h"
#include "model/shaders/simplelitshader.h"

namespace
{
    void loadFailsafeShaders()
    {
        Model::ResourceEnvironment* resourceEnv = Model::ResourceEnvironment::globalInstance();
        Model::ShaderStore* shaderStore = resourceEnv->shaderStore();

        quint16 failsafeShader = shaderStore->addShaderProgram<Model::SimpleLitShader>();
        Model::ShaderPalette palette;
        palette.addItem(Model::ShaderPalette::DefaultShader, failsafeShader);

        resourceEnv->setFailsafeShaderPalette(palette);
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
            Model::ResourceEnvironment::globalInitialise();

            loadFailsafeShaders();
            loadFailsafeTextures();
        }

        void shutDownSubSystems()
        {
            Model::ResourceEnvironment::globalShutdown();
        }
    }
}
