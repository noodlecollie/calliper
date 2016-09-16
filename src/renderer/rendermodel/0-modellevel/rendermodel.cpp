#include "rendermodel.h"

namespace NS_RENDERER
{
    RenderModel::RenderModel()
        : m_pShaderFunctor(NULL), m_pTextureFunctor(NULL)
    {
    }

    RenderModel::~RenderModel()
    {
        clearRenderPasses();
    }

    RenderModel::RenderModelPassPointer RenderModel::createRenderPass(const RenderModelPassKey &key)
    {
        RenderModelPassPointer pass(new RenderModelPass(m_pShaderFunctor));
        m_RenderPasses.insert(key, pass);
        return pass;
    }

    RenderModel::RenderModelPassPointer RenderModel::getRenderPass(const RenderModelPassKey &key) const
    {
        return m_RenderPasses.value(key, RenderModelPassPointer(NULL));
    }

    void RenderModel::removeRenderPass(const RenderModelPassKey &key)
    {
        m_RenderPasses.remove(key);
    }

    bool RenderModel::containsRenderPass(const RenderModelPassKey &key)
    {
        return m_RenderPasses.contains(key);
    }

    void RenderModel::clearRenderPasses()
    {
        m_RenderPasses.clear();
    }

    IShaderRetrievalFunctor* RenderModel::shaderFunctor()
    {
        return m_pShaderFunctor;
    }

    void RenderModel::setShaderFunctor(IShaderRetrievalFunctor *functor)
    {
        m_pShaderFunctor = functor;
    }

    ITextureRetrievalFunctor* RenderModel::textureFunctor()
    {
        return m_pTextureFunctor;
    }

    void RenderModel::setTextureFunctor(ITextureRetrievalFunctor *functor)
    {
        m_pTextureFunctor = functor;
    }
}
