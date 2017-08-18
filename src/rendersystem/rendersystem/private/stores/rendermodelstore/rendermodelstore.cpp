#include "rendermodelstore.h"

RenderModelStore::RenderModelStore()
    : ItemPointerBasedObjectStore<RenderModel,
                                  RenderSystem::RenderModelDefs::RenderModelId>(),
      m_ItemModelAdapter(*this)
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
                            const RenderSystem::FrameDrawParams &drawParams)
{
    QSharedPointer<RenderModel> model = object(modelId);
    if ( !model )
    {
        return;
    }

    model->draw(frameBufferId, drawParams);
}

QAbstractItemModel* RenderModelStore::itemModel() const
{
    return m_ItemModelAdapter.abstractItemModel();
}
