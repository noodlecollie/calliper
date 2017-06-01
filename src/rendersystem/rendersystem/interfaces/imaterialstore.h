#ifndef IMATERIALSTORE_H
#define IMATERIALSTORE_H

#include "rendersystem_global.h"

#include <QWeakPointer>

#include "rendersystem/interface-classes/definitions/materialdefs.h"
#include "rendersystem/interface-classes/rendermaterial/rendermaterial.h"

namespace RenderSystem
{
    class IMaterialStore
    {
    public:
        virtual ~IMaterialStore() {}

        // Creates a new blank material with the given path and returns its ID.
        virtual MaterialDefs::MaterialId createMaterial(const QString& strPath) = 0;

        // Removes the material with the given ID from the store.
        // If a material with this ID does not exist, this function does nothing.
        virtual void removeMaterial(const MaterialDefs::MaterialId id) = 0;

        // Returns the material corresponding to the given ID, or a default material
        // if the ID does not exist in the store.
        // Weak pointers are returned to indicate that the store still retains
        // full ownership of the material - don't store QSharedPointers.
        // The weak pointer is, however, guaranteed to be valid immediately after the call.
        virtual QWeakPointer<RenderMaterial> material(const MaterialDefs::MaterialId id) const = 0;
        virtual QWeakPointer<RenderMaterial> material(const QString& path) const = 0;

        // Returns whether a material with the given ID exists in the store.
        virtual bool containsMaterial(const MaterialDefs::MaterialId id) const = 0;

        // Returns whether a material with the giveb path exists.
        virtual bool containsMaterial(const QString& path) const = 0;

        // Returns the ID for the material with the given path, or 0 if it doesn't exist.
        virtual MaterialDefs::MaterialId materialIdFromPath(const QString& path) const = 0;

        // Returns the path for the material with the given ID, or 0 if it doesn't exist.
        virtual QString materialPathFromId(const MaterialDefs::MaterialId id) const = 0;
    };
}

#endif // IMATERIALSTORE_H
