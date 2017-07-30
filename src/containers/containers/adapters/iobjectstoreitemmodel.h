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

        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const = 0;
        virtual QModelIndex parent(const QModelIndex &index) const = 0;
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const = 0;
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const = 0;
        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const = 0;
    };
}

#endif // IOBJECTSTOREITEMMODEL_H
