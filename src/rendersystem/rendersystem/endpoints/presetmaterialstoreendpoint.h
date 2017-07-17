#ifndef PRESETMATERIALSTOREENDPOINT_H
#define PRESETMATERIALSTOREENDPOINT_H

#include "rendersystem_global.h"
#include "rendersystem/interface-classes/currentcontextguard/currentcontextguard.h"
#include "rendersystem/interfaces/ipresetmaterials.h"

namespace RenderSystem
{
    namespace PresetMaterialStoreEndpoint
    {
        typedef CurrentContextGuard<IPresetMaterials> PresetMaterialStoreAccessor;

        RENDERSYSTEMSHARED_EXPORT PresetMaterialStoreAccessor presetMaterialStore();
    }
}

#endif // PRESETMATERIALSTOREENDPOINT_H
