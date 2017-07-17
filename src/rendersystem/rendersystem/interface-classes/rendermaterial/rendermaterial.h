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

    private:
        const QString m_strPath;
        ShaderDefs::ShaderId m_nShaderId;

        QString m_TextureUnitMap[TextureDefs::TOTAL_TEXTURE_UNITS];
    };
}

#endif // RENDERMATERIAL_H
