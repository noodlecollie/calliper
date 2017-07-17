#include "rendermaterial.h"

namespace RenderSystem
{
    RenderMaterial::RenderMaterial(const QString& path)
        : m_strPath(path),
          m_nShaderId(ShaderDefs::UnknownShaderId),
          m_TextureUnitMap(),
          m_RenderTargetMap()
    {

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

    void RenderMaterial::addRenderTargetMapping(TextureDefs::TextureUnit textureUnit, TextureDefs::RenderTarget renderTarget)
    {
        m_RenderTargetMap.insert(textureUnit, renderTarge);
    }

    void RenderMaterial::removeTextureUnitMapping(TextureDefs::TextureUnit textureUnit)
    {
        m_RenderTargetMap.remove(textureUnit);
    }

    TextureDefs::RenderTarget RenderMaterial::renderTargetMapping(TextureDefs::TextureUnit textureUnit) const
    {
        return m_RenderTargetMap.value(textureUnit, TextureDefs::NoRenderTarget);
    }

    void RenderMaterial::clearRenderTargetMappings()
    {
        m_RenderTargetMap.clear();
    }
}
