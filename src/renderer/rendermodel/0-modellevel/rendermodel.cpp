#include "rendermodel.h"

namespace NS_RENDERER
{
    RenderModel::RenderModel()
        : m_pShaderFunctor(NULL), m_pTextureFunctor(NULL)
    {
    }

    RenderModel::~RenderModel()
    {
        clear();
    }

    void RenderModel::clear()
    {
        qDeleteAll(m_RenderPasses);
        m_RenderPasses.clear();
    }

    int RenderModel::createPass()
    {
        m_RenderPasses.append(new RenderModelPass(m_pShaderFunctor, m_pTextureFunctor));
        return m_RenderPasses.count() - 1;
    }

    int RenderModel::passCount() const
    {
        return m_RenderPasses.count();
    }

    void RenderModel::updateItem(quint64 objectUniqueId, const RenderModelInputParams &params, const GeometrySectionList &geometry)
    {
        Q_ASSERT_X(params.passIndex() < 0 || params.passIndex() >= m_RenderPasses.count(), Q_FUNC_INFO, "Pass is out of range!");

        // Update the table entry with the new params.
        m_ItemTable.insert(objectUniqueId, params);

        // Put the new data in the model.
        m_RenderPasses[params.passIndex()]->addItem(
                    RenderModelBatchKey(params.shaderId(), params.textureId(), params.modelToWorldMatrix(),
                                        params.drawMode(), params.drawWidth()),
                    RenderModelBatchParams(geometry, objectUniqueId, params.modelToWorldMatrix()));

    }

    void RenderModel::removeItem(quint64 objectUniqueId)
    {
        // TODO
        Q_UNUSED(objectUniqueId);
        Q_ASSERT(false);
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
