#ifndef MATERIALSTORE_H
#define MATERIALSTORE_H

#include <QHash>

#include "rendersystem/interface-classes/store-defs/publicstoredefs.h"

#include "rendersystem/private/store-classes/pathmanagingobjectstore.h"

#include "rendersystem/interface-classes/rendermaterial/rendermaterial.h"
#include "rendersystem/interfaces/imaterialstore.h"

class MaterialStore : public PathManagingObjectStore<RenderSystem::RenderMaterial, RenderSystem::PublicStoreDefs::MaterialId>,
                      public RenderSystem::IMaterialStore
{
public:
    typedef RenderSystem::PublicStoreDefs::MaterialId MaterialId;

    virtual MaterialId createMaterial(const QString& path) override;
    virtual void removeMaterial(const MaterialId id) override;
    virtual QWeakPointer<RenderSystem::RenderMaterial> material(const MaterialId id) const override;
    virtual bool containsMaterial(const MaterialId id) const override;
    virtual bool containsMaterial(const QString& path) const override;
    virtual MaterialId materialIdFromPath(const QString& path) const override;
    virtual QString materialPathFromId(const MaterialId id) const override;

protected:
    virtual void objectCreated(const ObjectId id) override;
    virtual void objectAboutToBeDestroyed(const ObjectId id) override;

private:
    QHash<QString, MaterialId> m_PathToMaterialIdMap;
};

#endif // MATERIALSTORE_H
