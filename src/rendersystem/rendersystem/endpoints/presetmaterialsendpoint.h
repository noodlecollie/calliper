#ifndef PRESETMATERIALSENDPOINT_H
#define PRESETMATERIALSENDPOINT_H

#include "rendersystem_global.h"
#include "rendersystem/interface-classes/currentcontextguard/currentcontextguard.h"
#include "rendersystem/interfaces/ipresetmaterials.h"

namespace RenderSystem
{
    namespace PresetMaterialsEndpoint
    {
        typedef CurrentContextGuard<IPresetMaterials> PresetMaterialsAccessor;

        RENDERSYSTEMSHARED_EXPORT PresetMaterialsAccessor presetMaterials();
    }
}

#endif // PRESETMATERIALSENDPOINT_H
