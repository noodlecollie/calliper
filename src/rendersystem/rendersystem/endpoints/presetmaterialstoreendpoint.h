#ifndef PRESETMATERIALSTOREENDPOINT_H
#define PRESETMATERIALSTOREENDPOINT_H

#include "rendersystem_global.h"
#include "rendersystem/interface-classes/currentcontextguard/currentcontextguard.h"
#include "rendersystem/interfaces/ipresetmaterialstore.h"

namespace RenderSystem
{
    namespace PresetMaterialStoreEndpoint
    {
        RENDERSYSTEMSHARED_EXPORT IPresetMaterialStore* presetMaterialStore();
        RENDERSYSTEMSHARED_EXPORT const IPresetMaterialStore* constPresetMaterialStore();
    }
}

#endif // PRESETMATERIALSTOREENDPOINT_H
