#include "rendermodelstoreitemmodeladapter.h"

#include "rendersystem/private/stores/rendermodelstore/rendermodelstore.h"

RenderModelStoreItemModelAdapter::RenderModelStoreItemModelAdapter(const RenderModelStore &store)
    : Containers::ItemPointerBasedItemModelAdapter<RenderModel,
                                                   RenderSystem::RenderModelDefs::RenderModelId>(&store),
      m_Store(store)
{

}

int RenderModelStoreItemModelAdapter::columnCount(const QModelIndex &parent) const
{
    if ( parent.isValid() )
    {
        return 0;
    }

    return TOTAL_MODEL_COLUMNS;
}

QVariant RenderModelStoreItemModelAdapter::itemData(const ObjectId &id, const QModelIndex &index, int role) const
{
    QSharedPointer<RenderModel> renderModel = m_Store.object(id);
    if ( !renderModel )
    {
        Q_ASSERT_X(false, Q_FUNC_INFO, "Expected valid render model!");
        return QVariant();
    }

    if ( role != Qt::DisplayRole )
    {
        return QVariant();
    }

    switch ( index.column() )
    {
        case ObjectIdColumn:
        {
            return QVariant::fromValue(id);
        }

        case NameColumn:
        {
            return QVariant(renderModel->name());
        }

        default:
        {
            break;
        }
    }

    return QVariant();
}

QVariant RenderModelStoreItemModelAdapter::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ( role != Qt::DisplayRole || orientation != Qt::Horizontal )
    {
        return objectStoreAbstractItemModel()->defaultHeaderData(section, orientation, role);
    }

    if ( section < ObjectIdColumn || section >= TOTAL_MODEL_COLUMNS )
    {
        return objectStoreAbstractItemModel()->defaultHeaderData(section, orientation, role);
    }

    switch ( section )
    {
        case ObjectIdColumn:
        {
            return QCoreApplication::translate("RenderModelStoreItemModelAdapter", "Object ID");
        }

        case NameColumn:
        {
            return QCoreApplication::translate("RenderModelStoreItemModelAdapter", "Name");
        }

        default:
        {
            return objectStoreAbstractItemModel()->defaultHeaderData(section, orientation, role);
        }
    }
}
