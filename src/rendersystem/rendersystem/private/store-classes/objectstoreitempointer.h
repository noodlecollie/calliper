#ifndef OBJECTSTOREITEMPOINTER_H
#define OBJECTSTOREITEMPOINTER_H

#include <QSharedPointer>

template<typename T, typename INTID>
class ObjectStoreItemPointer : public QSharedPointer<T>
{
public:
    typedef INTID ObjectId;

    template<typename... Args>
    ObjectStoreItemPointer(const ObjectId id, Args... args)
        : QSharedPointer<T>(new T(std::move(args)...)),
          m_nId(id)
    {
    }

    ObjectId objectId() const;

private:
    const ObjectId m_nId;
};

template<typename T, typename INTID>
typename ObjectStoreItemPointer<T, INTID>::ObjectId ObjectStoreItemPointer<T, INTID>::objectId() const
{
    return m_nId;
}

#endif // OBJECTSTOREITEMPOINTER_H
