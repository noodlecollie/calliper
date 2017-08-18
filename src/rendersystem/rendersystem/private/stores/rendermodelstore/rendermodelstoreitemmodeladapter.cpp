#include "rendermodelstoreitemmodeladapter.h"

#include <QApplication>

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
    if ( role != Qt::DisplayRole )
    {
        return abstractItemModel()->defaultHeaderData(section, orientation, role);
    }

    if ( orientation != Qt::Horizontal )
    {
        return abstractItemModel()->defaultHeaderData(section, orientation, role);
    }

    if ( section < 0 || section >= TOTAL_MODEL_COLUMNS )
    {
        return abstractItemModel()->defaultHeaderData(section, orientation, role);
    }

    switch ( section )
    {
        case NameColumn:
        {
            return QApplication::translate("RenderModelStoreItemModelAdapter", "Name");
        }

        default:
        {
            return abstractItemModel()->defaultHeaderData(section, orientation, role);
        }
    }
}
