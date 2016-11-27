#include "rendermaterial.h"

namespace Renderer
{
    RenderMaterial::RenderMaterial()
    {

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
}
