#include "resourceenvironment.h"

namespace Model
{
    ResourceEnvironment::ResourceEnvironment()
        : m_pMaterialResourceManager(nullptr),
          m_pShaderStore(nullptr)
    {
        m_pShaderStore = new ShaderStore();
        m_pMaterialResourceManager = new MaterialResourceManager();
    }

    ResourceEnvironment::~ResourceEnvironment()
    {
        delete m_pMaterialResourceManager;
        delete m_pShaderStore;
    }

    MaterialResourceManager* ResourceEnvironment::materialResourceManager()
    {
        return m_pMaterialResourceManager;
    }

    const MaterialResourceManager* ResourceEnvironment::materialResourceManager() const
    {
        return m_pMaterialResourceManager;
    }

    ShaderStore* ResourceEnvironment::shaderStore()
    {
        return m_pShaderStore;
    }

    const ShaderStore* ResourceEnvironment::shaderStore() const
    {
        return m_pShaderStore;
    }

    ShaderPalette ResourceEnvironment::failsafeShaderPalette() const
    {
        return m_FailsafePalette;
    }

    void ResourceEnvironment::setFailsafeShaderPalette(const ShaderPalette &palette)
    {
        m_FailsafePalette = palette;
    }
}
