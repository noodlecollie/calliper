#include "renderunit.h"

namespace Model
{
    RenderUnit::RenderUnit()
        : m_pShaderStore(Q_NULLPTR),
          m_pTextureStore(Q_NULLPTR),
          m_pMaterialStore(Q_NULLPTR),
          m_pShaderPaletteStore(Q_NULLPTR),
          m_pRenderModel(new Renderer::RenderModel())
    {
    }

    RenderUnit::~RenderUnit()
    {

    }

    ShaderStore* RenderUnit::shaderStore() const
    {
        return m_pShaderStore;
    }

    void RenderUnit::setShaderStore(ShaderStore* store)
    {
        m_pShaderStore = store;
        m_pRenderModel->setShaderFunctor(m_pShaderStore);
    }

    TextureStore* RenderUnit::textureStore() const
    {
        return m_pTextureStore;
    }

    void RenderUnit::setTextureStore(TextureStore* store)
    {
        m_pTextureStore = store;
        m_pRenderModel->setTextureFunctor(m_pTextureStore);
    }

    MaterialStore* RenderUnit::materialStore() const
    {
        return m_pMaterialStore;
    }

    void RenderUnit::setMaterialStore(MaterialStore* store)
    {
        m_pMaterialStore = store;
        m_pRenderModel->setMaterialFunctor(m_pMaterialStore);
    }

    ShaderPaletteStore* RenderUnit::shaderPaletteStore() const
    {
        return m_pShaderPaletteStore;
    }

    void RenderUnit::setShaderPaletteStore(ShaderPaletteStore* store)
    {
        m_pShaderPaletteStore = store;
    }

    Renderer::RenderModel* RenderUnit::renderModel() const
    {
        return m_pRenderModel;
    }
}
