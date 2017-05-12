#ifndef PATHMANAGINGOBJECTSTORE_H
#define PATHMANAGINGOBJECTSTORE_H

#include "rendersystem/private/store-classes/itempointerbasedobjectstore.h"
#include "rendersystem/private/store-classes/objectstoreitempointer.h"

template<typename T, typename INTID>
class PathManagingObjectStore : public ItemPointerBasedObjectStore<T, INTID>
{
public:
    typedef typename ItemPointerBasedObjectStore<T, INTID>::ObjectId ObjectId;

    bool contains(const QString& path) const;

    ObjectId objectIdFromPath(const QString& path) const;
    QString objectPathFromId(const ObjectId id) const;

protected:
    typedef ItemPointerBasedObjectStore<T, INTID> BaseItemPointerBasedObjectStore;

    template<typename... Args>
    ObjectId createWithPath(const QString& path, Args... args)
    {
        if ( path.isEmpty() )
        {
            return this->INVALID_ID;
        }

        return this->create(path, std::move(args)...);
    }

    void storePathForCreatedObject(const ObjectId id);
    void removePathForDestroyedObject(const ObjectId id);

private:
    QHash<QString, INTID> m_PathToObjectMap;
};

template<typename T, typename INTID>
void PathManagingObjectStore<T, INTID>::storePathForCreatedObject(const ObjectId id)
{
    // Don't store paths for the default object.
    if ( id == this->INVALID_ID )
    {
        return;
    }

    QSharedPointer<T> objectPointer = this->object(id);
    Q_ASSERT_X(objectPointer, Q_FUNC_INFO, "Invalid object!");
    if ( !objectPointer )
    {
        return;
    }

    Q_ASSERT_X(!m_PathToObjectMap.contains(objectPointer->path()), Q_FUNC_INFO, "Path for new object already exists!");

    m_PathToObjectMap.insert(objectPointer->path(), id);
}

template<typename T, typename INTID>
void PathManagingObjectStore<T, INTID>::removePathForDestroyedObject(const ObjectId id)
{
    if ( id == this->INVALID_ID )
    {
        return;
    }

    QSharedPointer<T> objectPointer = this->object(id);
    Q_ASSERT_X(objectPointer, Q_FUNC_INFO, "Invalid object!");
    if ( !objectPointer )
    {
        return;
    }

    m_PathToObjectMap.remove(objectPointer->path());
}

template<typename T, typename INTID>
typename ItemPointerBasedObjectStore<T, INTID>::ObjectId PathManagingObjectStore<T, INTID>::objectIdFromPath(const QString &path) const
{
    return m_PathToObjectMap.value(path, this->INVALID_ID);
}

template<typename T, typename INTID>
QString PathManagingObjectStore<T, INTID>::objectPathFromId(const ObjectId id) const
{
    QSharedPointer<T> objectPointer = this->object(id);
    Q_ASSERT_X(objectPointer, Q_FUNC_INFO, "Invaid object!");
    if ( !objectPointer )
    {
        return QString();
    }

    return objectPointer->path();
}

template<typename T, typename INTID>
bool PathManagingObjectStore<T, INTID>::contains(const QString& path) const
{
    return m_PathToObjectMap.contains(path);
}

#endif // PATHMANAGINGOBJECTSTORE_H
