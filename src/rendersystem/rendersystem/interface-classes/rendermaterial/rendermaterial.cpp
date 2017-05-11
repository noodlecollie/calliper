#include "rendermaterial.h"

namespace RenderSystem
{
    RenderMaterial::RenderMaterial(const QString& path)
        : m_strPath(path),
          m_nShaderStyle(PublicShaderDefs::UnknownShaderStyle),
          m_TextureUnitMap()
    {

    }

    QString RenderMaterial::path() const
    {
        return m_strPath;
    }

    PublicShaderDefs::ShaderStyle RenderMaterial::shaderStyle() const
    {
        return m_nShaderStyle;
    }

    void RenderMaterial::setShaderStyle(PublicShaderDefs::ShaderStyle style)
    {
        m_nShaderStyle = style;
    }

    void RenderMaterial::addTextureUnitMapping(PublicTextureDefs::TextureUnit textureUnit, const QString &texturePath)
    {
        if ( texturePath.isNull() )
        {
            return;
        }

        m_TextureUnitMap.insert(textureUnit, texturePath);
    }

    void RenderMaterial::removeTextureUnitMapping(PublicTextureDefs::TextureUnit textureUnit)
    {
        m_TextureUnitMap.remove(textureUnit);
    }

    QString RenderMaterial::textureMapping(PublicTextureDefs::TextureUnit textureUnit) const
    {
        return m_TextureUnitMap.value(textureUnit, QString());
    }

    void RenderMaterial::clearTextureMappings()
    {
        m_TextureUnitMap.clear();
    }
}
