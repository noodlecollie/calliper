#ifndef MATERIALSTORE_H
#define MATERIALSTORE_H

#include "rendersystem/interface-classes/store-defs/publicstoredefs.h"

#include "rendersystem/private/store-classes/itempointerbasedobjectstore.h"
#include "rendersystem/private/store-classes/objectstoreitempointer.h"

#include "rendersystem/interface-classes/rendermaterial/rendermaterial.h"

class MaterialStore : public ItemPointerBasedObjectStore<RenderSystem::RenderMaterial, RenderSystem::PublicStoreDefs::MaterialId>
{
protected:
    virtual void objectCreated(const ObjectId id) override;
    virtual void objectAboutToBeDestroyed(const ObjectId id) override;
};

#endif // MATERIALSTORE_H
