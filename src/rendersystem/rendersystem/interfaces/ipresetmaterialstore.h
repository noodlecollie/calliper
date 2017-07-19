#ifndef IPRESETMATERIALSTORE_H
#define IPRESETMATERIALSTORE_H

#include "rendersystem/interface-classes/definitions/materialdefs.h"

namespace RenderSystem
{
    class IPresetMaterialStore
    {
    public:
        virtual ~IPresetMaterialStore() {}

        // Returns the material store ID for the given preset material.
        virtual MaterialDefs::MaterialId material(const RenderSystem::MaterialDefs::PresetMaterial presetMaterial) const = 0;
    };
}

#endif // IPRESETMATERIALSTORE_H
