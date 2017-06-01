#include "rendermaterial.h"

namespace RenderSystem
{
    RenderMaterial::RenderMaterial(const QString& path)
        : m_strPath(path),
          m_nShaderStyle(ShaderDefs::UnknownShaderStyle),
          m_TextureUnitMap()
    {

    }

    QString RenderMaterial::path() const
    {
        return m_strPath;
    }

    ShaderDefs::ShaderStyle RenderMaterial::shaderStyle() const
    {
        return m_nShaderStyle;
    }

    void RenderMaterial::setShaderStyle(ShaderDefs::ShaderStyle style)
    {
        m_nShaderStyle = style;
    }

    void RenderMaterial::addTextureUnitMapping(TextureDefs::TextureUnit textureUnit, const QString &texturePath)
    {
        if ( texturePath.isNull() )
        {
            return;
        }

        m_TextureUnitMap.insert(textureUnit, texturePath);
    }

    void RenderMaterial::removeTextureUnitMapping(TextureDefs::TextureUnit textureUnit)
    {
        m_TextureUnitMap.remove(textureUnit);
    }

    QString RenderMaterial::textureMapping(TextureDefs::TextureUnit textureUnit) const
    {
        return m_TextureUnitMap.value(textureUnit, QString());
    }

    void RenderMaterial::clearTextureMappings()
    {
        m_TextureUnitMap.clear();
    }
}
