#ifndef RENDERUNIT_H
#define RENDERUNIT_H

#include "model_global.h"
#include "model/stores/shaderstore.h"
#include "model/stores/texturestore.h"
#include "model/stores/materialstore.h"
#include "model/stores/shaderpalettestore.h"
#include "renderer/rendermodel/0-modellevel/rendermodel.h"
#include "renderer/geometry/geometrybuilder.h"
#include <QSharedPointer>

namespace Model
{
    class RenderUnit
    {
    public:
        RenderUnit();
        ~RenderUnit();

        ShaderStore* shaderStore() const;
        void setShaderStore(ShaderStore* store);

        TextureStore* textureStore() const;
        void setTextureStore(TextureStore* store);

        MaterialStore* materialStore() const;
        void setMaterialStore(MaterialStore* store);

        ShaderPaletteStore* shaderPaletteStore() const;
        void setShaderPaletteStore(ShaderPaletteStore* store);

        Renderer::RenderModel* renderModel() const;

    private:
        ShaderStore* m_pShaderStore;
        TextureStore* m_pTextureStore;
        MaterialStore* m_pMaterialStore;
        ShaderPaletteStore* m_pShaderPaletteStore;

        Renderer::RenderModel* m_pRenderModel;
    };
}

#endif // RENDERUNIT_H
