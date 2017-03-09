#ifndef RENDERMATERIAL_H
#define RENDERMATERIAL_H

#include "renderer_global.h"
#include <QMap>
#include "renderer/shaders/shaderdefs.h"
#include "renderer/shaders/baseshaderpalette.h"

namespace Renderer
{
    class RENDERERSHARED_EXPORT RenderMaterial
    {
    public:
        RenderMaterial(quint32 id, const QString& path);

        quint32 materialStoreId() const;
        QString path() const;

        ShaderDefs::ShaderTechnique shaderTechnique() const;
        void setShaderTechnique(ShaderDefs::ShaderTechnique technique);

        void addTexture(ShaderDefs::TextureUnit textureUnit, quint32 textureStoreId);
        quint32 texture(ShaderDefs::TextureUnit textureUnit) const;
        int textureCount() const;
        QList<ShaderDefs::TextureUnit> textureUnits() const;
        QList<quint32> textures() const;
        void clearTextures();

        bool hasBaseTexture() const;

        const QMap<ShaderDefs::TextureUnit, quint32>& textureUnitMap() const;

    private:
        quint32 m_iId;
        QString m_strPath;
        QMap<ShaderDefs::TextureUnit, quint32> m_TextureUnitToIdMap;
        ShaderDefs::ShaderTechnique m_nTechnique;
    };
}

#endif // RENDERMATERIAL_H
