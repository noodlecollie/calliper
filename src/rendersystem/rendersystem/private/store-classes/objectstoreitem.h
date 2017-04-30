#ifndef OBJECTSTOREITEM_H
#define OBJECTSTOREITEM_H

template<typename T, typename INTID>
class ObjectStoreItem
{
public:
    typedef INTID ObjectId;

    template<typename... Args>
    ObjectStoreItem(const ObjectId id, Args... args)
        : m_nId(id),
          m_Object(std::move(args)...)
    {
    }

    ObjectId objectId() const;

    T& item();
    const T& item() const;

    T& operator *();
    const T& operator *() const;

    T* operator ->();
    const T* operator ->() const;

private:
    const ObjectId m_nId;
    T m_Object;
};

template<typename T>
typename ObjectStoreItem<T>::ObjectId ObjectStoreItem<T>::objectId() const
{
    return m_nId;
}

template<typename T>
T& ObjectStoreItem<T>::item()
{
    return m_Object;
}

template<typename T>
const T& ObjectStoreItem<T>::item() const
{
    return m_Object;
}

template<typename T>
T& ObjectStoreItem<T>::operator *()
{
    return item();
}

template<typename T>
const T& ObjectStoreItem<T>::operator *() const
{
    return item();
}

template<typename T>
T* ObjectStoreItem<T>::operator ->()
{
    return &m_Object;
}

template<typename T>
const T* ObjectStoreItem<T>::operator ->() const
{
    return &m_Object;
}

#endif // OBJECTSTOREITEM_H
