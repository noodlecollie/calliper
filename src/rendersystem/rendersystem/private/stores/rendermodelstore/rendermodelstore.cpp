#include "rendermodelstore.h"

RenderModelStore::RenderModelStore()
    : ItemPointerBasedObjectStore<RenderModel,
                                  RenderSystem::RenderModelDefs::RenderModelId>()
{

}

RenderModelStore::~RenderModelStore()
{

}

RenderModelStore::RenderModelId RenderModelStore::createRenderModel()
{
    return create();
}

void RenderModelStore::removeRenderModel(RenderModelId modelId)
{
    destroy(modelId);
}

void RenderModelStore::addGeometry(RenderModelId modelId, const RenderSystem::GeometryBuilder& builder)
{
    QSharedPointer<RenderModel> model = object(modelId);
    if ( !model )
    {
        return;
    }

    model->setGeometry(builder);
}

void RenderModelStore::removeGeometry(RenderModelId modelId, ObjectId objectId)
{
    QSharedPointer<RenderModel> model = object(modelId);
    if ( !model )
    {
        return;
    }

    model->removeGeometry(objectId);
}

void RenderModelStore::clearGeometry(RenderModelId modelId)
{
    QSharedPointer<RenderModel> model = object(modelId);
    if ( !model )
    {
        return;
    }

    model->clear();
}

void RenderModelStore::draw(const RenderModelId modelId,
                            const FrameBufferId frameBufferId,
                            const RenderMode renderMode,
                            const QMatrix4x4 &worldToCameraMatrix,
                            const QMatrix4x4 &projectionMatrix)
{
    QSharedPointer<RenderModel> model = object(modelId);
    if ( !model )
    {
        return;
    }

    model->draw(frameBufferId, renderMode, worldToCameraMatrix, projectionMatrix);
}
