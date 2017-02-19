#ifndef RESOURCEENVIRONMENT_H
#define RESOURCEENVIRONMENT_H

#include "model_global.h"
#include "calliperutil/global/globalinstance.h"
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

    private:
        // These are allocated on the heap so we get to control
        // their construction/destruction order.
        ShaderStore* m_pShaderStore;
        TextureStore* m_pTextureStore;
        MaterialStore* m_pMaterialStore;

        ShaderPalette m_FailsafePalette;
    };

    typedef CalliperUtil::GlobalInstance<ResourceEnvironment> ResourceEnvironmentInstance;
}

#endif // RESOURCEENVIRONMENT_H
