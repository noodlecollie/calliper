#ifndef RENDERMATERIAL_H
#define RENDERMATERIAL_H

#include "rendersystem_global.h"

#include <QHash>

#include "rendersystem/interface-classes/definitions/texturedefs.h"
#include "rendersystem/interface-classes/definitions/shaderdefs.h"

namespace RenderSystem
{
    class RENDERSYSTEMSHARED_EXPORT RenderMaterial
    {
    public:
        RenderMaterial(const QString& path);

        QString path() const;

        ShaderDefs::ShaderId shaderId() const;
        void setShaderId(ShaderDefs::ShaderId shader);

        void addTextureUnitMapping(TextureDefs::TextureUnit textureUnit, const QString& texturePath);
        void removeTextureUnitMapping(TextureDefs::TextureUnit textureUnit);
        QString textureMapping(TextureDefs::TextureUnit textureUnit) const;
        void clearTextureMappings();

        // Render target mappings override texture paths.
        void addRenderTargetMapping(TextureDefs::TextureUnit textureUnit, TextureDefs::RenderTarget renderTarget);
        void removeRenderTargetMapping(TextureDefs::TextureUnit textureUnit);
        TextureDefs::RenderTarget renderTargetMapping(TextureDefs::TextureUnit textureUnit) const;
        void clearRenderTargetMappings();

    private:
        const QString m_strPath;
        ShaderDefs::ShaderId m_nShaderId;
        QHash<TextureDefs::TextureUnit, QString> m_TextureUnitMap;
        QHash<TextureDefs::TextureUnit, TextureDefs::RenderTarget> m_RenderTargetMap;
    };
}

#endif // RENDERMATERIAL_H
