#ifndef IOBJECTSTOREITEMMODEL_H
#define IOBJECTSTOREITEMMODEL_H

#include <QAbstractItemModel>

namespace Containers
{
    // Interface that adapters can implement. This means the adapters can be templated,
    // and the actual QAbstractItemModel subclass can be passed a pointer to this interface,
    // which avoids the "no templated QObject subclasses" issue.
    class IObjectStoreItemModel
    {
    public:
        virtual ~IObjectStoreItemModel() {}

        // All the functions that the QAbstractItemModel needs to call on the implementer of this interface.
        // The implementer should return the appropriate values, which will be passed back up through the QAbstractItemModel.
        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const = 0;
        virtual QModelIndex parent(const QModelIndex &index) const = 0;
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const = 0;
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const = 0;
        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const = 0;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const = 0;
        virtual QVariant objectIdForRow(int row) const = 0;

        // The actual QAbstractItemModel that has a pointer to this interface.
        // Be careful not to get into infinite recursion here!
        virtual const QAbstractItemModel* abstractItemModel() const = 0;
        virtual QAbstractItemModel* abstractItemModel() = 0;
    };
}

#endif // IOBJECTSTOREITEMMODEL_H
