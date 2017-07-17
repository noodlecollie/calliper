#ifndef MATERIALDEFS_H
#define MATERIALDEFS_H

#include "rendersystem_global.h"

#include <QtGlobal>

namespace RenderSystem
{
    namespace MaterialDefs
    {
        typedef quint32 MaterialId;

        RENDERSYSTEMSHARED_EXPORT extern const MaterialId INVALID_MATERIAL_ID;

        enum PresetMaterial
        {
            UnlitMaterial = 0,

            TOTAL_PRESET_MATERIALS
        };
    }
}

#endif // MATERIALDEFS_H
