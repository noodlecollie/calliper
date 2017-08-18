#ifndef OBJECTSTOREITEMMODELADAPTER_H
#define OBJECTSTOREITEMMODELADAPTER_H

#include <QVector>
#include <QString>
#include <QTextStream>
#include <QCoreApplication>

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

        // The object ID should always be found at column 0.
        static constexpr int ObjectIdColumn = 0;

        ObjectStoreItemModelAdapter(const StoreType* store);

        virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
        virtual QModelIndex parent(const QModelIndex &index) const override;
        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        virtual QVariant objectIdForRow(int row) const override;
        const QAbstractItemModel* abstractItemModel() const override;
        QAbstractItemModel* abstractItemModel() override;

        // headerData() is not implemented because it's not required here.

        const ObjectStoreAbstractItemModel* objectStoreAbstractItemModel() const;
        ObjectStoreAbstractItemModel* objectStoreAbstractItemModel();

    protected:
        // Should return a QVariant to represent the object at the given ID, with the given role.
        // Note that column 0 with a role of Qt::DisplayRole will be handled automatically
        // by this class from within data().
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
    QVariant ObjectStoreItemModelAdapter<T>::objectIdForRow(int row) const
    {
        return data(index(row, ObjectIdColumn));
    }

    template<typename T>
    ObjectStoreAbstractItemModel* ObjectStoreItemModelAdapter<T>::objectStoreAbstractItemModel()
    {
        return &m_AbstractItemModel;
    }

    template<typename T>
    const ObjectStoreAbstractItemModel* ObjectStoreItemModelAdapter<T>::objectStoreAbstractItemModel() const
    {
        return &m_AbstractItemModel;
    }

    template<typename T>
    QAbstractItemModel* ObjectStoreItemModelAdapter<T>::abstractItemModel()
    {
        return objectStoreAbstractItemModel();
    }

    template<typename T>
    const QAbstractItemModel* ObjectStoreItemModelAdapter<T>::abstractItemModel() const
    {
        return objectStoreAbstractItemModel();
    }
}

#endif // OBJECTSTOREITEMMODELADAPTER_H
