#ifndef ITEMPOINTERBASEDOBJECTSTORE_H
#define ITEMPOINTERBASEDOBJECTSTORE_H

#include "containers/objectstore/objectstore.h"
#include "objectstoreitempointer.h"

namespace Containers
{
    template<typename T, typename INTID>
    class ItemPointerBasedObjectStore : public ObjectStore<ObjectStoreItemPointer<T, INTID> >
    {
    public:
        typedef typename ObjectStore<ObjectStoreItemPointer<T, INTID> >::ObjectId ObjectId;

        ItemPointerBasedObjectStore();
        virtual ~ItemPointerBasedObjectStore();

        QSharedPointer<T> object(ObjectId id) const;

    protected:
        typedef ObjectStoreItemPointer<T, INTID> BaseObjectStoreItemPointer;
        typedef ObjectStore<BaseObjectStoreItemPointer> BaseObjectStore;
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
        return this->m_ObjectHash.value(id, ObjectStoreItemPointer<T, INTID>());
    }
}

#endif // ITEMPOINTERBASEDOBJECTSTORE_H
