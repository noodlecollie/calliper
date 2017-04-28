#ifndef OBJECTSTOREITEMPOINTER_H
#define OBJECTSTOREITEMPOINTER_H

#include "rendersystem_global.h"

#include <QSharedPointer>

namespace RenderSystem
{
    template<typename T>
    class ObjectStoreItemPointer : public QSharedPointer<T>
    {
    public:
        typedef typename T::ObjectId ObjectId;

        template<typename... Args>
        ObjectStoreItemPointer(const typename T::ObjectId id, Args... args)
            : QSharedPointer<T>(new T(id, std::move(args)...))
        {
        }
    };
}

#endif // OBJECTSTOREITEMPOINTER_H
