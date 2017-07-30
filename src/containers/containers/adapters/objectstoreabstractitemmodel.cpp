#include "objectstoreabstractitemmodel.h"

namespace Containers
{
    ObjectStoreAbstractItemModel::ObjectStoreAbstractItemModel(const IObjectStoreItemModel *adapterInterface, QObject *parent)
        : QAbstractItemModel(parent),
          m_pAdapterInterface(adapterInterface)
    {

    }

    QModelIndex ObjectStoreAbstractItemModel::index(int row, int column, const QModelIndex &parent) const
    {
        if ( !m_pAdapterInterface )
        {
            return QModelIndex();
        }

        return m_pAdapterInterface->index(row, column, parent);
    }

    QModelIndex ObjectStoreAbstractItemModel::parent(const QModelIndex &index) const
    {
        if ( !m_pAdapterInterface )
        {
            return QModelIndex();
        }

        return m_pAdapterInterface->parent(index);
    }

    int ObjectStoreAbstractItemModel::rowCount(const QModelIndex &parent) const
    {
        if ( !m_pAdapterInterface )
        {
            return 0;
        }

        return m_pAdapterInterface->rowCount(parent);
    }

    int ObjectStoreAbstractItemModel::columnCount(const QModelIndex &parent) const
    {
        if ( !m_pAdapterInterface )
        {
            return 0;
        }

        return m_pAdapterInterface->columnCount(parent);
    }

    QVariant ObjectStoreAbstractItemModel::data(const QModelIndex &index, int role) const
    {
        if ( !m_pAdapterInterface )
        {
            return QVariant();
        }

        return m_pAdapterInterface->data(index, role);
    }

    QVariant ObjectStoreAbstractItemModel::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if ( !m_pAdapterInterface )
        {
            return QVariant();
        }

        return m_pAdapterInterface->headerData(section, orientation, role);
    }

    QModelIndex ObjectStoreAbstractItemModel::createIndexHelper(int row, int column, quintptr id) const
    {
        return createIndex(row, column, id);
    }

    QVariant ObjectStoreAbstractItemModel::defaultHeaderData(int section, Qt::Orientation orientation, int role) const
    {
        return QAbstractItemModel::headerData(section, orientation, role);
    }

    void ObjectStoreAbstractItemModel::notifyDataChanged()
    {
        beginResetModel();
        endResetModel();
    }
}
