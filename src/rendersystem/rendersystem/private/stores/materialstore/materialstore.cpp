#include "materialstore.h"

MaterialStore::MaterialId MaterialStore::createMaterial(const QString& path)
{
    MaterialId existingMaterial = m_PathToMaterialIdMap.value(path, INVALID_ID);
    if ( existingMaterial != INVALID_ID )
    {
        destroy(existingMaterial);
    }

    return create(path);
}

void MaterialStore::removeMaterial(const MaterialId id)
{
    if ( id == INVALID_ID )
    {
        return;
    }

    destroy(id);
}

QWeakPointer<RenderSystem::RenderMaterial> MaterialStore::material(const MaterialId id) const
{
    return object(id).toWeakRef();
}

bool MaterialStore::containsMaterial(const MaterialId id) const
{
    return BaseObjectStore::contains(id);
}

bool MaterialStore::containsMaterial(const QString &path) const
{
    return contains(path);
}

MaterialStore::MaterialId MaterialStore::materialIdFromPath(const QString& path) const
{
    return objectIdFromPath(path);
}

QString MaterialStore::materialPathFromId(const MaterialId id) const
{
    return objectPathFromId(id);
}

void MaterialStore::objectCreated(const ObjectId id)
{
    storePathForCreatedObject(id);
}

void MaterialStore::objectAboutToBeDestroyed(const ObjectId id)
{
    removePathForDestroyedObject(id);
}
