#ifndef RESOURCEENVIRONMENT_H
#define RESOURCEENVIRONMENT_H

#include "model_global.h"
#include "materialresourcemanager.h"
#include "shaderstore.h"
#include "model/shaders/shaderpalette.h"

namespace Model
{
    class MODELSHARED_EXPORT ResourceEnvironment
    {
    public:
        ResourceEnvironment();
        ~ResourceEnvironment();

        MaterialResourceManager* materialResourceManager();
        const MaterialResourceManager* materialResourceManager() const;

        ShaderStore* shaderStore();
        const ShaderStore* shaderStore() const;

        ShaderPalette failsafeShaderPalette() const;
        void setFailsafeShaderPalette(const ShaderPalette& palette);

    private:
        // These are allocated on the heap so we get to control
        // their construction/destruction order.
        MaterialResourceManager* m_pMaterialResourceManager;
        ShaderStore* m_pShaderStore;

        ShaderPalette m_FailsafePalette;
    };
}

#endif // RESOURCEENVIRONMENT_H
