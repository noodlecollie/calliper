#ifndef RESOURCEENVIRONMENT_H
#define RESOURCEENVIRONMENT_H

#include "model_global.h"

#include "renderer/functors/renderfunctorgroup.h"

#include "renderer/shaders/baseshaderpalette.h"
#include "model/stores/shaderstore.h"
#include "model/stores/texturestore.h"
#include "model/stores/materialstore.h"
#include "model/stores/shaderpalettestore.h"
#include "model/scenerenderer/sceneframerenderer.h"

namespace Model
{
    class MODELSHARED_EXPORT ResourceEnvironment
    {
    public:
        ResourceEnvironment();
        ~ResourceEnvironment();

        ShaderStore* shaderStore();
        const ShaderStore* shaderStore() const;

        TextureStore* textureStore();
        const TextureStore* textureStore() const;

        MaterialStore* materialStore();
        const MaterialStore* materialStore() const;

        ShaderPaletteStore* shaderPaletteStore();
        const ShaderPaletteStore* shaderPaletteStore() const;

        SceneFrameRenderer* sceneFrameRenderer();
        const SceneFrameRenderer* sceneFrameRenderer() const;

        Renderer::RenderFunctorGroup renderFunctors() const;

        static void globalInitialise();
        static void globalShutdown();
        static ResourceEnvironment* globalInstance();

    private:
        static ResourceEnvironment* m_pGlobalInstance;

        // These are allocated on the heap so we get to control
        // their construction/destruction order.
        ShaderStore* m_pShaderStore;
        TextureStore* m_pTextureStore;
        MaterialStore* m_pMaterialStore;
        ShaderPaletteStore* m_pShaderPaletteStore;
        SceneFrameRenderer* m_pSceneFrameRenderer;
    };
}

#endif // RESOURCEENVIRONMENT_H
