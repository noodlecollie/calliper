#ifndef RESOURCEENVIRONMENT_H
#define RESOURCEENVIRONMENT_H

#include "model_global.h"
#include "model/stores/shaderstore.h"
#include "model/stores/texturestore.h"
#include "model/stores/materialstore.h"
#include "model/shaders/shaderpalette.h"

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

        ShaderPalette failsafeShaderPalette() const;
        void setFailsafeShaderPalette(const ShaderPalette& palette);

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

        ShaderPalette m_FailsafePalette;
    };
}

#endif // RESOURCEENVIRONMENT_H
