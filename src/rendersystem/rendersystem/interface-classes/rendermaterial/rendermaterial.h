#ifndef RENDERMATERIAL_H
#define RENDERMATERIAL_H

#include "rendersystem_global.h"

#include <QHash>

#include "rendersystem/interface-classes/texture/publictexturedefs.h"
#include "rendersystem/interface-classes/shader/publicshaderdefs.h"

namespace RenderSystem
{
    class RENDERSYSTEMSHARED_EXPORT RenderMaterial
    {
    public:
        RenderMaterial();

        PublicShaderDefs::ShaderStyle shaderStyle() const;
        void setShaderStyle(PublicShaderDefs::ShaderStyle style);

        void addTextureUnitMapping(PublicTextureDefs::TextureUnit textureUnit, const QString& texturePath);
        void removeTextureUnitMapping(PublicTextureDefs::TextureUnit textureUnit);
        QString textureMapping(PublicTextureDefs::TextureUnit textureUnit) const;
        void clearTextureMappings();

    private:
        PublicShaderDefs::ShaderStyle m_nShaderStyle;
        QHash<PublicTextureDefs::TextureUnit, QString> m_TextureUnitMap;
    };
}

#endif // RENDERMATERIAL_H
