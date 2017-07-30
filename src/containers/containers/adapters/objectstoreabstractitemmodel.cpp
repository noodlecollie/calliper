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
}
