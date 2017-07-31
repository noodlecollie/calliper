#ifndef OBJECTSTOREITEMMODELADAPTER_H
#define OBJECTSTOREITEMMODELADAPTER_H

#include <QVector>
#include <QString>
#include <QTextStream>

#include "containers/objectstore/objectstore.h"

#include "changereceiver.h"
#include "iobjectstoreitemmodel.h"
#include "objectstoreabstractitemmodel.h"

namespace Containers
{
    template<typename T>
    class ObjectStoreItemModelAdapter : public IObjectStoreItemModel
    {
    public:
        typedef typename T::ObjectId ObjectId;
        typedef ObjectStore<T> StoreType;

        ObjectStoreItemModelAdapter(const StoreType* store);

        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
        virtual QModelIndex parent(const QModelIndex &index) const override;
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

        // Should return a QVariant to represent the header data for the given section, with the given role.
        // The default implementation just calls through to the abstract item model.
        virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

        const ObjectStoreAbstractItemModel* abstractItemModel() const;
        ObjectStoreAbstractItemModel* abstractItemModel();

    protected:
        // Should return a QVariant to represent the object at the given ID, with the given role.
        virtual QVariant itemData(const ObjectId& id, const QModelIndex &index, int role) const = 0;

    private:
        void changeNotificationReceived();

        const StoreType* m_pObjectStore;
        QVector<ObjectId> m_IndexToId;
        ChangeReceiver m_ChangeReceiver;
        ObjectStoreAbstractItemModel m_AbstractItemModel;
    };

    template<typename T>
    ObjectStoreItemModelAdapter<T>::ObjectStoreItemModelAdapter(const StoreType *store)
        : m_pObjectStore(store),
          m_IndexToId(),
          m_ChangeReceiver(),
          m_AbstractItemModel(this)
    {
        m_ChangeReceiver.setChangeCallback([this]
        {
            this->changeNotificationReceived();
        });

        if ( !m_pObjectStore )
        {
            return;
        }

        m_ChangeReceiver.connect(m_pObjectStore->changeNotifier(), &ChangeNotifier::storeChanged, &m_ChangeReceiver, &ChangeReceiver::changeNotificationReceived);
    }

    template<typename T>
    void ObjectStoreItemModelAdapter<T>::changeNotificationReceived()
    {
        if ( !m_pObjectStore )
        {
            return;
        }

        m_IndexToId.clear();
        m_IndexToId.reserve(m_pObjectStore->count());

        for ( typename StoreType::ConstIterator it = m_pObjectStore->constBegin(); it != m_pObjectStore->constEnd(); ++it )
        {
            m_IndexToId.append(it.key());
        }

        std::sort(m_IndexToId.begin(), m_IndexToId.end());

        m_AbstractItemModel.notifyDataChanged();
    }

    template<typename T>
    QModelIndex ObjectStoreItemModelAdapter<T>::index(int row, int column, const QModelIndex &parent) const
    {
        if ( !m_pObjectStore )
        {
            return QModelIndex();
        }

        if ( parent.isValid() || row < 0 || row >= m_pObjectStore->count() || column < 0 || column >= columnCount() )
        {
            return QModelIndex();
        }

        return m_AbstractItemModel.createIndexHelper(row, column, 0);
    }

    template<typename T>
    QModelIndex ObjectStoreItemModelAdapter<T>::parent(const QModelIndex &index) const
    {
        Q_UNUSED(index);
        return QModelIndex();
    }

    template<typename T>
    int ObjectStoreItemModelAdapter<T>::rowCount(const QModelIndex &parent) const
    {
        Q_UNUSED(parent);

        if ( !m_pObjectStore )
        {
            return 0;
        }

        return m_IndexToId.count();
    }

    template<typename T>
    int ObjectStoreItemModelAdapter<T>::columnCount(const QModelIndex &parent) const
    {
        Q_UNUSED(parent);

        if ( !m_pObjectStore )
        {
            return 0;
        }

        return 1;
    }

    template<typename T>
    QVariant ObjectStoreItemModelAdapter<T>::data(const QModelIndex &index, int role) const
    {
        if ( !m_pObjectStore )
        {
            return QVariant();
        }

        if ( index.row() < 0 || index.row() >= m_IndexToId.count() || index.column() < 0 || index.column() >= columnCount() )
        {
            return QVariant();
        }

        return itemData(m_IndexToId.at(index.row()), index, role);
    }

    template<typename T>
    ObjectStoreAbstractItemModel* ObjectStoreItemModelAdapter<T>::abstractItemModel()
    {
        return &m_AbstractItemModel;
    }

    template<typename T>
    const ObjectStoreAbstractItemModel* ObjectStoreItemModelAdapter<T>::abstractItemModel() const
    {
        return &m_AbstractItemModel;
    }

    template<typename T>
    QVariant ObjectStoreItemModelAdapter<T>::headerData(int section, Qt::Orientation orientation, int role) const
    {
        return m_AbstractItemModel.headerData(section, orientation, role);
    }
}

#endif // OBJECTSTOREITEMMODELADAPTER_H
