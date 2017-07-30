#ifndef OBJECTSTOREABSTRACTITEMMODEL_H
#define OBJECTSTOREABSTRACTITEMMODEL_H

#include "containers_global.h"

#include <QAbstractItemModel>

#include "iobjectstoreitemmodel.h"

namespace Containers
{
    class CONTAINERSSHARED_EXPORT ObjectStoreAbstractItemModel : public QAbstractItemModel
    {
        Q_OBJECT
    public:
        explicit ObjectStoreAbstractItemModel(const IObjectStoreItemModel* adapterInterface, QObject *parent = 0);

        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
        virtual QModelIndex parent(const QModelIndex &index) const override;
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

        QModelIndex createIndexHelper(int row, int column, quintptr id) const;
        QVariant defaultHeaderData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

        void notifyDataChanged();

    private:
        const IObjectStoreItemModel* m_pAdapterInterface;
    };
}

#endif // OBJECTSTOREABSTRACTITEMMODEL_H
