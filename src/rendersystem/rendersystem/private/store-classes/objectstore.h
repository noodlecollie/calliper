#ifndef OBJECTSTORE_H
#define OBJECTSTORE_H

#include <QHash>

// Suitable for use with ObjectStoreItem or ObjectStoreItemPointer.
template<typename T>
class ObjectStore
{
public:
    typedef typename T::ObjectId ObjectId;
    static const ObjectId INVALID_ID;

    ObjectStore();
    virtual ~ObjectStore();

    template<typename... Args>
    ObjectId create(Args... args)
    {
        ObjectId nextId = acquireNextId();
        if ( nextId == INVALID_ID )
        {
            return INVALID_ID;
        }

        m_ObjectHash.insert(T(nextId, std::move(args)...));
        objectCreated(nextId);
        return nextId;
    }

    bool destroy(ObjectId id);
    bool contains(ObjectId id) const;
    void clear();
    int count() const;

protected:
    virtual void objectCreated(const ObjectId id);
    virtual void objectAboutToBeDestroyed(const ObjectId id);

    QHash<ObjectId, T> m_ObjectHash;

private:
    ObjectId acquireNextId();

    ObjectId m_nIdCounter;
};

template<typename T>
const typename ObjectStore<T>::ObjectId ObjectStore<T>::INVALID_ID = 0;

template<typename T>
ObjectStore<T>::ObjectStore()
    : m_nIdCounter(INVALID_ID),
      m_ObjectHash()
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

#endif // OBJECTSTORE_H
