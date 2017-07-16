#include "materialstore.h"

MaterialStore::MaterialStore()
    : PathManagingObjectStore<RenderSystem::RenderMaterial,
                              RenderSystem::MaterialDefs::MaterialId>()
{
    createDefaultMaterial();
    createDebugMinimalMaterial();
}

MaterialStore::~MaterialStore()
{
    // No resources need to be cleaned up in the material store.
    m_ObjectHash.clear();
}

MaterialStore::MaterialId MaterialStore::createMaterial(const QString& path)
{
    if ( path.isEmpty() )
    {
        return INVALID_ID;
    }

    MaterialId existingMaterial = objectIdFromPath(path);
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

QWeakPointer<RenderSystem::RenderMaterial> MaterialStore::material(const QString &path) const
{
    return object(materialIdFromPath(path)).toWeakRef();
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

void MaterialStore::createDefaultMaterial()
{
    createDefaultObject(QString());
}

void MaterialStore::createDebugMinimalMaterial()
{
    MaterialId id = createMaterial("_debug/debugminimal");
    QWeakPointer<RenderSystem::RenderMaterial> dbgMaterialWeak = material(id);
    QSharedPointer<RenderSystem::RenderMaterial> dbgMaterial = dbgMaterialWeak.toStrongRef();

    dbgMaterial->setShaderId(RenderSystem::ShaderDefs::DebugMinimalShaderId);
}
