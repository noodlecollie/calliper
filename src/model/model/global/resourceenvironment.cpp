#include "resourceenvironment.h"

namespace Model
{
    ResourceEnvironment* ResourceEnvironment::m_pGlobalInstance = Q_NULLPTR;

    void ResourceEnvironment::globalInitialise()
    {
        Q_ASSERT_X(!m_pGlobalInstance, Q_FUNC_INFO, "Global instance already initialised!");
        if ( m_pGlobalInstance )
        {
            return;
        }

        m_pGlobalInstance = new ResourceEnvironment();
    }

    void ResourceEnvironment::globalShutdown()
    {
        Q_ASSERT_X(m_pGlobalInstance, Q_FUNC_INFO, "Global instance not initialised!");

        delete m_pGlobalInstance;
        m_pGlobalInstance = Q_NULLPTR;
    }

    ResourceEnvironment* ResourceEnvironment::globalInstance()
    {
        return m_pGlobalInstance;
    }

    ResourceEnvironment::ResourceEnvironment()
        : m_pShaderStore(Q_NULLPTR),
          m_pTextureStore(Q_NULLPTR),
          m_pMaterialStore(Q_NULLPTR)
    {
        m_pShaderStore = new ShaderStore();
        m_pTextureStore = new TextureStore();
        m_pMaterialStore = new MaterialStore();
    }

    ResourceEnvironment::~ResourceEnvironment()
    {
        delete m_pMaterialStore;
        delete m_pTextureStore;
        delete m_pShaderStore;
    }

    ShaderStore* ResourceEnvironment::shaderStore()
    {
        return m_pShaderStore;
    }

    const ShaderStore* ResourceEnvironment::shaderStore() const
    {
        return m_pShaderStore;
    }

    TextureStore* ResourceEnvironment::textureStore()
    {
        return m_pTextureStore;
    }

    const TextureStore* ResourceEnvironment::textureStore() const
    {
        return m_pTextureStore;
    }

    MaterialStore* ResourceEnvironment::materialStore()
    {
        return m_pMaterialStore;
    }

    const MaterialStore* ResourceEnvironment::materialStore() const
    {
        return m_pMaterialStore;
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
