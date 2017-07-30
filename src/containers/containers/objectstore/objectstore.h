#ifndef OBJECTSTORE_H
#define OBJECTSTORE_H

#include <QHash>

#include "changenotifier.h"

namespace Containers
{
    // Suitable for use with ObjectStoreItem or ObjectStoreItemPointer.
    template<typename T>
    class ObjectStore
    {
    public:
        typedef typename T::ObjectId ObjectId;
        static const ObjectId INVALID_ID;

        ObjectStore();
        virtual ~ObjectStore();

        bool destroy(ObjectId id);
        bool contains(ObjectId id) const;
        void clear();
        int count() const;

        const ChangeNotifier* changeNotifier() const;

        typedef typename QHash<ObjectId, T>::const_iterator ConstIterator;
        ConstIterator constBegin() const;
        ConstIterator constEnd() const;

    protected:
        typedef QHash<ObjectId, T> ObjectStoreHash;

        template<typename... Args>
        ObjectId create(Args... args)
        {
            ObjectId nextId = acquireNextId();
            if ( nextId == INVALID_ID )
            {
                return INVALID_ID;
            }

            m_ObjectHash.insert(nextId, T(nextId, std::move(args)...));
            m_ChangeNotifier.notifyStoreChanged();

            objectCreated(nextId);
            return nextId;
        }

        template<typename... Args>
        bool replace(ObjectId id, Args... args)
        {
            if ( id == INVALID_ID || !m_ObjectHash.contains(id) )
            {
                return false;
            }

            objectAboutToBeDestroyed(id);
            m_ObjectHash.insert(id, T(id, std::move(args)...));
            m_ChangeNotifier.notifyStoreChanged();

            objectCreated(id);
            return true;
        }

        template<typename... Args>
        ObjectId createDefaultObject(Args... args)
        {
            if ( m_ObjectHash.contains(INVALID_ID) )
            {
                Q_ASSERT_X(false, Q_FUNC_INFO, "Default object already exists!");
                return INVALID_ID;
            }

            m_ObjectHash.insert(INVALID_ID, T(INVALID_ID, std::move(args)...));
            m_ChangeNotifier.notifyStoreChanged();

            objectCreated(INVALID_ID);
            return INVALID_ID;
        }

        virtual void objectCreated(const ObjectId id);
        virtual void objectAboutToBeDestroyed(const ObjectId id);

        ObjectStoreHash m_ObjectHash;

    private:
        ObjectId acquireNextId();

        ObjectId m_nIdCounter;
        ChangeNotifier m_ChangeNotifier;
    };

    template<typename T>
    const typename ObjectStore<T>::ObjectId ObjectStore<T>::INVALID_ID = 0;

    template<typename T>
    ObjectStore<T>::ObjectStore()
        : m_ObjectHash(),
          m_nIdCounter(INVALID_ID)
    {

    }

    template<typename T>
    ObjectStore<T>::~ObjectStore()
    {
        foreach ( ObjectId id, m_ObjectHash.keys() )
        {
            destroy(id);
        }
    }

    template<typename T>
    bool ObjectStore<T>::destroy(ObjectId id)
    {
        if ( !contains(id) )
        {
            return false;
        }

        objectAboutToBeDestroyed(id);
        m_ObjectHash.remove(id);
        m_ChangeNotifier.notifyStoreChanged();

        return true;
    }

    template<typename T>
    bool ObjectStore<T>::contains(ObjectId id) const
    {
        return m_ObjectHash.contains(id);
    }

    template<typename T>
    void ObjectStore<T>::clear()
    {
        m_ObjectHash.clear();
        m_ChangeNotifier.notifyStoreChanged();
    }

    template<typename T>
    int ObjectStore<T>::count() const
    {
        return m_ObjectHash.count();
    }

    template<typename T>
    void ObjectStore<T>::objectCreated(const ObjectId id)
    {
        Q_UNUSED(id);
    }

    template<typename T>
    void ObjectStore<T>::objectAboutToBeDestroyed(const ObjectId id)
    {
        Q_UNUSED(id);
    }

    template<typename T>
    typename ObjectStore<T>::ObjectId ObjectStore<T>::acquireNextId()
    {
        ++m_nIdCounter;

        Q_ASSERT_X(m_nIdCounter != INVALID_ID, Q_FUNC_INFO, "Object ID overflow!");
        return m_nIdCounter;
    }

    template<typename T>
    typename ObjectStore<T>::ConstIterator ObjectStore<T>::constBegin() const
    {
        return m_ObjectHash.constBegin();
    }

    template<typename T>
    typename ObjectStore<T>::ConstIterator ObjectStore<T>::constEnd() const
    {
        return m_ObjectHash.constEnd();
    }

    template<typename T>
    const ChangeNotifier* ObjectStore<T>::changeNotifier() const
    {
        return &m_ChangeNotifier;
    }
}

#endif // OBJECTSTORE_H
