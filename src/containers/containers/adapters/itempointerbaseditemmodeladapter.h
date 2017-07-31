#ifndef ITEMPOINTERBASEDITEMMODELADAPTER_H
#define ITEMPOINTERBASEDITEMMODELADAPTER_H

#include "objectstoreitemmodeladapter.h"

#include "containers/itempointer/objectstoreitempointer.h"

namespace Containers
{
    template<typename T, typename INTID>
    class ItemPointerBasedItemModelAdapter : public ObjectStoreItemModelAdapter<ObjectStoreItemPointer<T, INTID>>
    {
    public:
        typedef typename ObjectStoreItemModelAdapter<ObjectStoreItemPointer<T, INTID>>::StoreType StoreType;

        ItemPointerBasedItemModelAdapter(const StoreType* store)
            : ObjectStoreItemModelAdapter<ObjectStoreItemPointer<T, INTID>>(store)
        {
        }
    };
}

#endif // ITEMPOINTERBASEDITEMMODELADAPTER_H
