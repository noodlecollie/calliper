#include "rendermodelprofilingwrapper.h"

#include "rendersystem/endpoints/rendermodelstoreendpoint.h"

RenderModelProfilingWrapper::RenderModelProfilingWrapper(QObject *parent)
    : QAbstractItemModel(parent),
      m_RenderModelStoreItemModel(*RenderSystem::RenderModelStoreEndpoint::constRenderModelStore()->itemModel())
{

}

QModelIndex RenderModelProfilingWrapper::index(int row, int column, const QModelIndex &parent) const
{
    if ( !parent.isValid() )
    {
        return getIndexForTopLevelItem(row, column);
    }

    // TODO
    return QModelIndex();
}

QModelIndex RenderModelProfilingWrapper::parent(const QModelIndex &index) const
{
    // TODO
    return QModelIndex();
}

int RenderModelProfilingWrapper::rowCount(const QModelIndex &parent) const
{
    // TODO
    return 0;
}

int RenderModelProfilingWrapper::columnCount(const QModelIndex &parent) const
{
    // TODO
    return 0;
}

QVariant RenderModelProfilingWrapper::data(const QModelIndex &index, int role) const
{
    // TODO
    return QVariant();
}

QModelIndex RenderModelProfilingWrapper::getIndexForTopLevelItem(int row, int column) const
{
    // TODO
    return QModelIndex;
}
