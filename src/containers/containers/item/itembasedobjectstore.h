#ifndef ITEMBASEDOBJECTSTORE_H
#define ITEMBASEDOBJECTSTORE_H

#include "containers/objectstore/objectstore.h"
#include "objectstoreitem.h"

namespace Containers
{
    // The type T must be default-constructable. A default-constructed
    // instance is assumed to correspond to a "null" instance.
    template<typename T, typename INTID>
    class ItemBasedObjectStore : public ObjectStore<ObjectStoreItem<T, INTID> >
    {
    public:
        typedef typename ObjectStore<ObjectStoreItem<T> >::ObjectId ObjectId;

        ItemBasedObjectStore();
        virtual ~ItemBasedObjectStore();

        T object(ObjectId id) const;
    };

    template<typename T, typename INTID>
    ItemBasedObjectStore<T, INTID>::ItemBasedObjectStore()
        : ObjectStore<ObjectStoreItem<T> >()
    {

    }

    template<typename T, typename INTID>
    ItemBasedObjectStore<T, INTID>::~ItemBasedObjectStore()
    {

    }

    template<typename T, typename INTID>
    T ItemBasedObjectStore<T, INTID>::object(ObjectId id) const
    {
        return this->m_ObjectHash.value(id, T());
    }
}

#endif // ITEMBASEDOBJECTSTORE_H
