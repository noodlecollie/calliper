#ifndef OBJECTSTORE_H
#define OBJECTSTORE_H

#include "rendersystem_global.h"

#include <QHash>

namespace RenderSystem
{
    // Type T should be an object, not a pointer. It should implement a default constructor
    // that returns a null instance of the object. It should also implement one or more
    // constructors that take an ID as the first argument, and any number of other arguments.
    // If a pointer is required, a wrapper class should be implemented first.
    // Type ID should be some kind of integer - an unsigned type is recommended.
    template<typename T, typename INTID>
    class ObjectStore
    {
    public:
        static const INTID INVALID_ID;

        ObjectStore();
        virtual ~ObjectStore();

        template<typename... Args>
        INTID createObject(Args... args)
        {
            INTID nextId = acquireNextId();
            if ( nextId == INVALID_ID )
            {
                return INVALID_ID;
            }

            m_ObjectHash.insert(T(nextId, std::move(args)...));
            objectCreated(nextId);
            return nextId;
        }

        bool destroyObject(INTID id);
        bool containsObject(INTID id) const;
        T object(INTID id) const;

    protected:
        virtual void objectCreated(const INTID id);
        virtual void objectAboutToBeDestroyed(const INTID id);

    private:
        INTID acquireNextId();

        INTID m_nIdCounter;
        QHash<INTID, T> m_ObjectHash;
    };

    template<typename T, typename INTID>
    const INTID ObjectStore<T,INTID>::INVALID_ID = 0;

    template<typename T, typename INTID>
    ObjectStore<T,INTID>::ObjectStore()
        : m_nIdCounter(INVALID_ID)
    {

    }

    template<typename T, typename INTID>
    ObjectStore<T,INTID>::~ObjectStore()
    {
        foreach ( INTID id, m_ObjectHash.keys() )
        {
            destroyObject(id);
        }
    }

    template<typename T, typename INTID>
    bool ObjectStore<T,INTID>::destroyObject(INTID id)
    {
        if ( !containsObject(id) )
        {
            return false;
        }

        objectAboutToBeDestroyed(id);
        m_ObjectHash.remove(id);
        return true;
    }

    template<typename T, typename INTID>
    bool ObjectStore<T,INTID>::containsObject(INTID id) const
    {
        return m_ObjectHash.contains(id);
    }

    template<typename T, typename INTID>
    T ObjectStore<T,INTID>::object(INTID id) const
    {
        return m_ObjectHash.value(id, T());
    }

    template<typename T, typename INTID>
    void ObjectStore<T,INTID>::objectCreated(const INTID id)
    {
        Q_UNUSED(id);
    }

    template<typename T, typename INTID>
    void ObjectStore<T,INTID>::objectAboutToBeDestroyed(const INTID id)
    {
        Q_UNUSED(id);
    }

    template<typename T, typename INTID>
    INTID ObjectStore<T,INTID>::acquireNextId()
    {
        ++m_nIdCounter;

        Q_ASSERT_X(m_nIdCounter != INVALID_ID, Q_FUNC_INFO, "Object ID overflow!");
        return m_nIdCounter;
    }
}

#endif // OBJECTSTORE_H
