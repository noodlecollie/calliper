#include "rendermaterial.h"

namespace Renderer
{
    RenderMaterial::RenderMaterial(quint32 id, const QString &path)
        : m_iId(id),
          m_strPath(path),
          m_TextureUnitToIdMap(),
          m_nTechnique(BaseShaderPalette::UnlitTextured)
    {

    }

    QString RenderMaterial::path() const
    {
        return m_strPath;
    }

    quint32 RenderMaterial::materialStoreId() const
    {
        return m_iId;
    }

    BaseShaderPalette::ShaderTechnique RenderMaterial::shaderTechnique() const
    {
        return m_nTechnique;
    }

    void RenderMaterial::setShaderTechnique(BaseShaderPalette::ShaderTechnique technique)
    {
        m_nTechnique = technique;
    }

    void RenderMaterial::addTexture(ShaderDefs::TextureUnit textureUnit, quint32 textureStoreId)
    {
        m_TextureUnitToIdMap.insert(textureUnit, textureStoreId);
    }

    quint32 RenderMaterial::texture(ShaderDefs::TextureUnit textureUnit) const
    {
        return m_TextureUnitToIdMap.value(textureUnit, 0);
    }

    int RenderMaterial::textureCount() const
    {
        return m_TextureUnitToIdMap.count();
    }

    void RenderMaterial::clearTextures()
    {
        m_TextureUnitToIdMap.clear();
    }

    const QMap<ShaderDefs::TextureUnit, quint32>& RenderMaterial::textureUnitMap() const
    {
        return m_TextureUnitToIdMap;
    }

    QList<quint32> RenderMaterial::textures() const
    {
        return m_TextureUnitToIdMap.values();
    }

    QList<ShaderDefs::TextureUnit> RenderMaterial::textureUnits() const
    {
        return m_TextureUnitToIdMap.keys();
    }

    bool RenderMaterial::hasBaseTexture() const
    {
        return m_TextureUnitToIdMap.contains(ShaderDefs::MainTexture);
    }
}
