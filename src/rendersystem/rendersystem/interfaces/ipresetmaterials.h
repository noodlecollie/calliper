#ifndef IPRESETMATERIALS_H
#define IPRESETMATERIALS_H

#include "rendersystem/interface-classes/definitions/materialdefs.h"

namespace RenderSystem
{
    class IPresetMaterials
    {
    public:
        virtual ~IPresetMaterials() {}

        // Returns the material store ID for the given preset material.
        virtual MaterialDefs::MaterialId material(const RenderSystem::MaterialDefs::PresetMaterial presetMaterial) const = 0;
    };
}

#endif // IPRESETMATERIALS_H
