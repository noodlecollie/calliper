#include "rendermodel.h"
#include <QtDebug>
#include "opengl/openglerrors.h"
#include "rendermodel/rendererobjectflags.h"

namespace
{
    void copy(NS_RENDERER::OpenGLUniformBuffer &dest, const QMatrix4x4 &src, int &offset)
    {
        using namespace NS_RENDERER;

        GLTRY(dest.write(offset, src.constData(), 16 * sizeof(float)));
        offset += 16 * sizeof(float);
    }

    inline bool flagIsSet(quint32 value, quint32 flag)
    {
        return (value & flag) == flag;
    }

    inline bool flagWasSet(quint32 before, quint32 after, quint32 flag)
    {
        return !flagIsSet(before, flag) && flagIsSet(after, flag);
    }

    inline bool flagWasUnset(quint32 before, quint32 after, quint32 flag)
    {
        return flagWasSet(after, before, flag);
    }
}

namespace NS_RENDERER
{
    RenderModel::RenderModel()
        : m_pShaderFunctor(nullptr), m_pTextureFunctor(nullptr), m_DrawParams(),
          m_GlobalUniformBuffer(QOpenGLBuffer::DynamicDraw), m_bUniformDataUploaded(false)
    {
        m_GlobalUniformBuffer.create();
    }

    RenderModel::~RenderModel()
    {
        clearRenderPasses();
        m_GlobalUniformBuffer.destroy();
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
        // This could be optimised by checking whether any keys matched
        // the ones incoming in this update, and only clearing (not
        // removing) entries for the ones that match.
        // For now, we just remove any previous entries.
        removeObject(object.objectId());

        // Don't bother going through all this if we don't actually have any geometry.
        bool allEmpty = true;
        foreach ( GeometrySection* section, object.geometrySectionList() )
        {
            if ( !section->isEmpty() )
            {
                allEmpty = false;
                break;
            }
        }

        if ( allEmpty )
            return;

        RenderModelPassKey passKey(object.passIndex());
        bool drawable = true;
        if ( m_ObjectFlags.contains(object.objectId()) )
        {
            drawable = (m_ObjectFlags.value(object.objectId()) & HiddenObjectFlag) == HiddenObjectFlag;
        }

        RenderModelKeyListPointer list = RenderModelKeyListPointer::create();

        quint8 sectionId = 0;
        foreach ( GeometrySection* section, object.geometrySectionList() )
        {
            RenderModelKey key = RenderModelKey(
                            passKey,
                            RenderModelBatchGroupKey(
                                section->shaderId(),
                                section->textureId(),
                                section->drawMode(),
                                section->drawWidth()
                            ),
                            MatrixBatchKey(section->modelToWorldMatrix()),
                            MatrixBatchItemKey(object.objectId(), sectionId)
                        );

            RenderModelPass::RenderModelBatchGroupPointer batchGroup;
            MatrixBatch::MatrixBatchItemPointer batchItem = createOrFetchMatrixBatchItem(key, &batchGroup);
            batchItem->clear();

            section->consolidate(batchItem->m_Positions,
                                batchItem->m_Normals,
                                batchItem->m_Colors,
                                batchItem->m_TextureCoordinates,
                                batchItem->m_Indices);

            list->append(key);
            batchGroup->setMatrixBatchDrawable(key.matrixBatchKey(), drawable);

            sectionId++;
        }

        m_StoredObjects.insert(object.objectId(), list);
        m_ObjectFlags.insert(object.objectId(), NoObjectFlag);
    }

    void RenderModel::removeObject(quint32 objectId)
    {
        if ( m_StoredObjects.contains(objectId) )
        {
            RenderModelKeyListPointer keysForObject = m_StoredObjects.take(objectId);
            foreach ( const RenderModelKey &key, *keysForObject.data() )
            {
                cleanMatrixBatchItem(key);
            }
        }

        m_ObjectFlags.remove(objectId);
    }

    bool RenderModel::getModelItems(const RenderModelKey &key,
                                    RenderModelPassPointer &pass,
                                    RenderModelPass::RenderModelBatchGroupPointer &batchGroup,
                                    RenderModelBatchGroup::MatrixBatchPointer &matrixBatch,
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
        if ( matrixBatch.isNull() )
        {
            return false;
        }

        batchItem = matrixBatch->getItem(key.matrixBatchItemKey());
        return !batchItem.isNull();
    }

    MatrixBatch::MatrixBatchItemPointer RenderModel::createOrFetchMatrixBatchItem(const RenderModelKey &key,
                                                     RenderModelPass::RenderModelBatchGroupPointer* batchGroupOut)
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
        if ( batchGroup )
        {
            *batchGroupOut = batchGroup;
        }

        // 3: Get the batch.
        RenderModelBatchGroup::MatrixBatchPointer matrixBatch = batchGroup->getMatrixBatch(key.matrixBatchKey());
        if ( matrixBatch.isNull() )
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

    void RenderModel::printDebugInfo() const
    {
        qDebug() << "Passes:" << m_RenderPasses.count();
    }

    void RenderModel::cleanMatrixBatchItem(const RenderModelKey &key)
    {
        RenderModelPassPointer pass;
        RenderModelPass::RenderModelBatchGroupPointer batchGroup;
        RenderModelBatchGroup::MatrixBatchPointer matrixBatch;
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

    void RenderModel::draw(const RendererDrawParams &params)
    {
        if ( params != m_DrawParams )
        {
            m_DrawParams = params;
            m_bUniformDataUploaded = false;
        }

        uploadGlobalUniformData();

        foreach ( const RenderModelPassPointer &pass, m_RenderPasses.values() )
        {
            pass->drawAllBatchGroups();
        }
    }

    void RenderModel::uploadGlobalUniformData()
    {
        if ( m_bUniformDataUploaded )
            return;

        GLTRY(m_GlobalUniformBuffer.bind());
        GLTRY(m_GlobalUniformBuffer.allocate(m_DrawParams.size()));
        GLTRY(m_GlobalUniformBuffer.release());

        GLTRY(m_GlobalUniformBuffer.bindToIndex(ShaderDefs::GlobalUniformBlockBindingPoint));

        GLTRY(m_GlobalUniformBuffer.bind());
        int offset = 0;

        copy(m_GlobalUniformBuffer, m_DrawParams.worldToCameraMatrix(), offset);
        copy(m_GlobalUniformBuffer, m_DrawParams.projectionMatrix(), offset);

        GLTRY(m_GlobalUniformBuffer.release());

        m_bUniformDataUploaded = true;
    }

    void RenderModel::setObjectFlags(quint32 objectId, quint32 flags)
    {
        if ( !m_ObjectFlags.contains(objectId) )
            return;

        quint32 oldObjFlags = m_ObjectFlags.value(objectId);
        quint32 newObjFlags = oldObjFlags | flags;
        m_ObjectFlags.insert(objectId, newObjFlags);

        if ( flagWasSet(oldObjFlags, newObjFlags, HiddenObjectFlag) )
        {
            setObjectHidden(objectId, true);
        }
    }

    void RenderModel::clearObjectFlags(quint32 objectId, quint32 flags)
    {
        if ( !m_ObjectFlags.contains(objectId) )
            return;

        quint32 oldObjFlags = m_ObjectFlags.value(objectId);
        quint32 newObjFlags = oldObjFlags & ~flags;
        m_ObjectFlags.insert(objectId, newObjFlags);

        if ( flagWasUnset(oldObjFlags, newObjFlags, HiddenObjectFlag) )
        {
            setObjectHidden(objectId, false);
        }
    }

    quint32 RenderModel::getObjectFlags(quint32 objectId) const
    {
        return m_ObjectFlags.value(objectId, NoObjectFlag);
    }

    void RenderModel::setObjectHidden(quint32 objectId, bool hidden)
    {
        Q_ASSERT_X(m_StoredObjects.contains(objectId), Q_FUNC_INFO, "Object does not exist!");
        RenderModelKeyListPointer list = m_StoredObjects.value(objectId);

        foreach ( const RenderModelKey &key, *(list.data()) )
        {
            RenderModelPassPointer pass = getRenderPass(key.passKey());
            if ( pass.isNull() )
                continue;

            RenderModelPass::RenderModelBatchGroupPointer batchGroup = pass->getBatchGroup(key.batchGroupKey());
            if ( batchGroup.isNull() )
                continue;

            batchGroup->setMatrixBatchDrawable(key.matrixBatchKey(), !hidden);
        }
    }
}
