#ifndef IMATERIALSTORE_H
#define IMATERIALSTORE_H

#include "rendersystem_global.h"

#include <QWeakPointer>

#include "rendersystem/interface-classes/store-defs/publicstoredefs.h"
#include "rendersystem/interface-classes/rendermaterial/rendermaterial.h"

namespace RenderSystem
{
    class IMaterialStore
    {
    public:
        virtual ~IMaterialStore() {}

        // Creates a new blank material with the given path and returns its ID.
        virtual PublicStoreDefs::MaterialId createMaterial(const QString& strPath) = 0;

        // Removes the material with the given ID from the store.
        // If a material with this ID does not exist, this function does nothing.
        virtual void removeMaterial(const PublicStoreDefs::MaterialId id) = 0;

        // Returns the material corresponding to the given ID, or a null pointer
        // if a material with the given ID does not exist.
        virtual QWeakPointer<RenderMaterial> material(const PublicStoreDefs::MaterialId id) const = 0;

        // Returns whether a material with the given ID exists in the store.
        virtual bool containsMaterial(const PublicStoreDefs::MaterialId id) const = 0;
        // TODO: A path version of this.

        // Returns the ID for the material with the given path, or 0 if it doesn't exist.
        virtual PublicStoreDefs::MaterialId materialIdFromPath(const QString& path) const = 0;

        // Returns the path for the material with the given ID, or 0 if it doesn't exist.
        virtual QString materialPathFromId(const PublicStoreDefs::MaterialId id) const = 0;
    };
}

#endif // IMATERIALSTORE_H
