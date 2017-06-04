#ifndef OBJECTSTOREITEMPOINTER_H
#define OBJECTSTOREITEMPOINTER_H

#include <QSharedPointer>

namespace Containers
{
    template<typename T, typename INTID>
    class ObjectStoreItemPointer : public QSharedPointer<T>
    {
    public:
        typedef INTID ObjectId;
        static const ObjectId INVALID_ID;

        template<typename... Args>
        ObjectStoreItemPointer(const ObjectId id, Args... args)
            : QSharedPointer<T>(new T(std::move(args)...)),
              m_nId(id)
        {
        }

        ObjectStoreItemPointer()
            : QSharedPointer<T>(),
              m_nId(INVALID_ID)
        {
        }

        ObjectId objectId() const;

    private:
        ObjectId m_nId;
    };

    template<typename T, typename INTID>
    const typename ObjectStoreItemPointer<T, INTID>::ObjectId ObjectStoreItemPointer<T, INTID>::INVALID_ID = 0;

    template<typename T, typename INTID>
    typename ObjectStoreItemPointer<T, INTID>::ObjectId ObjectStoreItemPointer<T, INTID>::objectId() const
    {
        return m_nId;
    }
}

#endif // OBJECTSTOREITEMPOINTER_H
