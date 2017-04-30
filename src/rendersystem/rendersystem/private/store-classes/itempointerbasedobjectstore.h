#ifndef ITEMPOINTERBASEDOBJECTSTORE_H
#define ITEMPOINTERBASEDOBJECTSTORE_H

#include "objectstore.h"
#include "objectstoreitempointer.h"

template<typename T, typename INTID>
class ItemPointerBasedObjectStore : public ObjectStore<ObjectStoreItemPointer<T, INTID> >
{
public:
    typedef typename ObjectStore<ObjectStoreItemPointer<T, INTID> >::ObjectId ObjectId;

    ItemPointerBasedObjectStore();
    virtual ~ItemPointerBasedObjectStore();

    QSharedPointer<T> object(ObjectId id) const;
};

template<typename T, typename INTID>
ItemPointerBasedObjectStore<T, INTID>::ItemPointerBasedObjectStore()
    : ObjectStore<ObjectStoreItemPointer<T, INTID> >()
{

}

template<typename T, typename INTID>
ItemPointerBasedObjectStore<T, INTID>::~ItemPointerBasedObjectStore()
{

}

template<typename T, typename INTID>
QSharedPointer<T> ItemPointerBasedObjectStore<T, INTID>::object(ObjectId id) const
{
    // This has to be fully qualified or it's treated as undeclared..?
    return ObjectStore<ObjectStoreItemPointer<T, INTID> >::m_ObjectHash.value(id, QSharedPointer<T>());
}

#endif // ITEMPOINTERBASEDOBJECTSTORE_H
