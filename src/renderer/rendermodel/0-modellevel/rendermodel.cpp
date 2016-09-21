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

    void RenderModel::updateObject(const RendererInputObjectParams &object)
    {
        MatrixBatch::MatrixBatchItemPointer batchItem = createOrFetchMatrixBatchItem(object.renderModelKey());
        batchItem->clear();

        foreach ( const GeometrySection &section, object.geometrySectionList() )
        {
            section.consolidate(batchItem->m_Positions,
                                batchItem->m_Normals,
                                batchItem->m_Colors,
                                batchItem->m_TextureCoordinates,
                                batchItem->m_Indices);
        }
    }

    MatrixBatch::MatrixBatchItemPointer RenderModel::createOrFetchMatrixBatchItem(const RenderModelKey &key)
    {
        // 1: Get the pass.
        RenderModelPassPointer pass = getRenderPass(key.passKey());
        if ( pass.isNull() )
        {
            pass = createRenderPass(key.passKey());
        }

        // 2: Get the batch group.
        // At some point we'll probably want to deal with usage patterns too.
        RenderModelPass::RenderModelBatchGroupPointer batchGroup = pass->getBatchGroup(key.batchGroupKey());
        if ( batchGroup.isNull() )
        {
            batchGroup = pass->createBatchGroup(key.batchGroupKey());
        }

        // 3: Get the batch.
        MatrixBatch* matrixBatch = batchGroup->getMatrixBatch(key.matrixBatchKey());
        if ( !matrixBatch )
        {
            matrixBatch = batchGroup->createMatrixBatch(key.matrixBatchKey());
        }

        // 4: Get the batch item.
        MatrixBatch::MatrixBatchItemPointer batchItem = matrixBatch->getItem(key.matrixBatchItemKey());
        if ( batchItem.isNull() )
        {
            batchItem = matrixBatch->createItem(key.matrixBatchItemKey());
        }

        return batchItem;
    }
}
