#include "rendermodel.h"
#include <QtDebug>

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
        RenderModelPassPointer pass = RenderModelPassPointer::create(m_pShaderFunctor, m_pTextureFunctor);
        m_RenderPasses.insert(key, pass);
        return pass;
    }

    RenderModel::RenderModelPassPointer RenderModel::getRenderPass(const RenderModelPassKey &key) const
    {
        return m_RenderPasses.value(key, RenderModelPassPointer());
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
        if ( m_StoredObjects.contains(object.objectId()) )
        {
            RenderModelKeyListPointer keysForObject = m_StoredObjects.take(object.objectId());
            foreach ( const RenderModelKey &key, *keysForObject.data() )
            {
                cleanMatrixBatchItem(key);
            }
        }

        RenderModelPassKey passKey(object.passIndex());
        MatrixBatchItemKey batchItemKey(object.objectId());

        foreach ( const GeometrySection &section, object.geometrySectionList() )
        {
            RenderModelKey key = RenderModelKey(
                            passKey,
                            RenderModelBatchGroupKey(
                                section.shaderId(),
                                section.textureId(),
                                section.drawMode(),
                                section.drawWidth()
                            ),
                            MatrixBatchKey(section.modelToWorldMatrix()),
                            batchItemKey
                        );

            MatrixBatch::MatrixBatchItemPointer batchItem = createOrFetchMatrixBatchItem(key);
            batchItem->clear();

            section.consolidate(batchItem->m_Positions,
                                batchItem->m_Normals,
                                batchItem->m_Colors,
                                batchItem->m_TextureCoordinates,
                                batchItem->m_Indices);

            qDebug() << "Updated batch item:";
            batchItem->printDebugInfo();
        }
    }

    bool RenderModel::getModelItems(const RenderModelKey &key,
                                    RenderModelPassPointer &pass,
                                    RenderModelPass::RenderModelBatchGroupPointer &batchGroup,
                                    MatrixBatch *&matrixBatch,
                                    MatrixBatch::MatrixBatchItemPointer &batchItem) const
    {
        pass = getRenderPass(key.passKey());
        if ( pass.isNull() )
        {
            return false;
        }

        batchGroup = pass->getBatchGroup(key.batchGroupKey());
        if ( batchGroup.isNull() )
        {
            return false;
        }

        matrixBatch = batchGroup->getMatrixBatch(key.matrixBatchKey());
        if ( !matrixBatch )
        {
            return false;
        }

        batchItem = matrixBatch->getItem(key.matrixBatchItemKey());
        return !batchItem.isNull();
    }

    MatrixBatch::MatrixBatchItemPointer RenderModel::createOrFetchMatrixBatchItem(const RenderModelKey &key)
    {
        // 1: Get the pass.
        RenderModelPassPointer pass = getRenderPass(key.passKey());
        if ( pass.isNull() )
        {
            pass = createRenderPass(key.passKey());
        }
        pass->printDebugInfo();

        // 2: Get the batch group.
        // At some point we'll probably want to deal with usage patterns too.
        RenderModelPass::RenderModelBatchGroupPointer batchGroup = pass->getBatchGroup(key.batchGroupKey());
        if ( batchGroup.isNull() )
        {
            batchGroup = pass->createBatchGroup(key.batchGroupKey());
        }
        batchGroup->printDebugInfo();

        // 3: Get the batch.
        MatrixBatch* matrixBatch = batchGroup->getMatrixBatch(key.matrixBatchKey());
        if ( !matrixBatch )
        {
            matrixBatch = batchGroup->createMatrixBatch(key.matrixBatchKey());
        }
        matrixBatch->printDebugInfo();

        // 4: Get the batch item.
        MatrixBatch::MatrixBatchItemPointer batchItem = matrixBatch->getItem(key.matrixBatchItemKey());
        if ( batchItem.isNull() )
        {
            batchItem = matrixBatch->createItem(key.matrixBatchItemKey());
        }
        batchItem->printDebugInfo();

        return batchItem;
    }

    void RenderModel::printDebugInfo() const
    {
        qDebug() << "Passes:" << m_RenderPasses.count();
    }

    void RenderModel::cleanMatrixBatchItem(const RenderModelKey &key)
    {
        RenderModelPassPointer pass;
        RenderModelPass::RenderModelBatchGroupPointer batchGroup;
        MatrixBatch* matrixBatch = NULL;
        MatrixBatch::MatrixBatchItemPointer batchItem;

        if ( !getModelItems(key, pass, batchGroup, matrixBatch, batchItem) )
        {
            return;
        }

        batchItem.clear();
        matrixBatch->removeItem(key.matrixBatchItemKey());

        if ( matrixBatch->itemCount() < 1 )
        {
            batchGroup->removeMatrixBatch(key.matrixBatchKey());
        }

        if ( batchGroup->matrixBatchCount() < 1 )
        {
            pass->removeBatchGroup(key.batchGroupKey());
        }
    }

    void RenderModel::draw()
    {
        // Iterate by key so we draw in order.
        foreach ( const RenderModelPassPointer &pass, m_RenderPasses.values() )
        {
            pass->drawAllBatchGroups();
        }
    }
}
