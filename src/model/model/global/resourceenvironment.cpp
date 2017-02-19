#include "resourceenvironment.h"

namespace Model
{
    ResourceEnvironment::ResourceEnvironment()
        : m_pShaderStore(nullptr),
          m_pTextureStore(nullptr),
          m_pMaterialStore(nullptr)
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
