#ifndef OBJECTSTORE_H
#define OBJECTSTORE_H

#include <QHash>

// Type T should be an object, not a pointer. It should implement a default constructor
// that returns a null instance of the object. It should also implement one or more
// constructors that take an ID as the first argument, and any number of other arguments.
// If a pointer is required, use ObjectStoreItemPointer.
template<typename T>
class ObjectStore
{
public:
    typedef typename T::ObjectId ObjectId;  // T::ObjectId should be an unsigned integral type.
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
    T object(ObjectId id) const;
    void clear();
    int count() const;

protected:
    virtual void objectCreated(const ObjectId id);
    virtual void objectAboutToBeDestroyed(const ObjectId id);

private:
    ObjectId acquireNextId();

    ObjectId m_nIdCounter;
    QHash<ObjectId, T> m_ObjectHash;
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
        destroyObject(id);
    }
}

template<typename T>
bool ObjectStore<T>::destroy(ObjectId id)
{
    if ( !containsObject(id) )
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
T ObjectStore<T>::object(ObjectId id) const
{
    return m_ObjectHash.value(id, T());
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
