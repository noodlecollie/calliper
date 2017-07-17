#include "rendermaterial.h"

namespace RenderSystem
{
    RenderMaterial::RenderMaterial(const QString& path)
        : m_strPath(path),
          m_nShaderId(ShaderDefs::UnknownShaderId),
          m_TextureUnitMap{}
    {
        clearTextureMappings();
    }

    QString RenderMaterial::path() const
    {
        return m_strPath;
    }

    ShaderDefs::ShaderId RenderMaterial::shaderId() const
    {
        return m_nShaderId;
    }

    void RenderMaterial::setShaderId(ShaderDefs::ShaderId shader)
    {
        m_nShaderId = shader;
    }

    void RenderMaterial::addTextureUnitMapping(TextureDefs::TextureUnit textureUnit, const QString &texturePath)
    {
        if ( texturePath.isNull() || textureUnit < 0 || textureUnit >= TextureDefs::TOTAL_TEXTURE_UNITS )
        {
            return;
        }

        m_TextureUnitMap[textureUnit] = texturePath;
    }

    void RenderMaterial::removeTextureUnitMapping(TextureDefs::TextureUnit textureUnit)
    {
        if ( textureUnit < 0 || textureUnit >= TextureDefs::TOTAL_TEXTURE_UNITS )
        {
            return;
        }

        m_TextureUnitMap[textureUnit] = QString();
    }

    QString RenderMaterial::textureMapping(TextureDefs::TextureUnit textureUnit) const
    {
        if ( textureUnit < 0 || textureUnit >= TextureDefs::TOTAL_TEXTURE_UNITS )
        {
            return QString();
        }

        return m_TextureUnitMap[textureUnit];
    }

    void RenderMaterial::clearTextureMappings()
    {
        for ( quint32 i = 0; i < TextureDefs::TOTAL_TEXTURE_UNITS; ++i )
        {
            m_TextureUnitMap[i] = QString();
        }
    }
}
