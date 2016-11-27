#ifndef RENDERMATERIAL_H
#define RENDERMATERIAL_H

#include "renderer_global.h"
#include <QMap>
#include "shaders/shaderdefs.h"

namespace Renderer
{
    class RenderMaterial
    {
    public:
        RenderMaterial();

        void addTexture(ShaderDefs::TextureUnit textureUnit, quint32 textureStoreId);
        quint32 texture(ShaderDefs::TextureUnit textureUnit) const;
        int textureCount() const;
        QList<ShaderDefs::TextureUnit> textureUnits() const;
        QList<quint32> textures() const;
        void clearTextures();

        const QMap<ShaderDefs::TextureUnit, quint32>& textureUnitMap() const;

    private:
        QMap<ShaderDefs::TextureUnit, quint32> m_TextureUnitToIdMap;
    };
}

#endif // RENDERMATERIAL_H
