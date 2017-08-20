#include "rendermodelstore.h"

RenderModelStore::RenderModelStore()
    : ItemPointerBasedObjectStore<RenderModel,
                                  RenderSystem::RenderModelDefs::RenderModelId>(),
      m_ItemModelAdapter(*this),
      m_ProfilingItemModelWrapper()
{

}

RenderModelStore::~RenderModelStore()
{

}

RenderModelStore::RenderModelId RenderModelStore::createRenderModel(const QString& name)
{
    return create(name);
}

QString RenderModelStore::name(const RenderModelId modelId) const
{
    QSharedPointer<RenderModel> model = object(modelId);
    if ( !model )
    {
        return QString();
    }

    return model->name();
}

void RenderModelStore::setName(const RenderModelId modelId, const QString &newName) const
{
    QSharedPointer<RenderModel> model = object(modelId);
    if ( !model )
    {
        return;
    }

    model->setName(newName);
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
    return m_ItemModelAdapter.objectStoreAbstractItemModel();
}

Profiling::ProfilerItemModelAdatper* RenderModelStore::profilingData(const RenderModelId modelId) const
{
    QSharedPointer<RenderModel> model = object(modelId);
    if ( !model )
    {
        return Q_NULLPTR;
    }

    return model->profilingData();
}

Containers::IObjectStoreItemModel* RenderModelStore::objectStoreItemModel() const
{
    return &m_ItemModelAdapter;
}

QAbstractItemModel* RenderModelStore::profilingItemModel() const
{
    return &m_ProfilingItemModelWrapper;
}
