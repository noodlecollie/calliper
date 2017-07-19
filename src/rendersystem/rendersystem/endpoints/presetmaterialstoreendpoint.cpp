#include "presetmaterialstoreendpoint.h"
#include "rendersystem/private/static-stores/presetmaterialstore/presetmaterialstore.h"

namespace RenderSystem
{
    namespace PresetMaterialStoreEndpoint
    {
        IPresetMaterialStore* presetMaterialStore()
        {
            return PresetMaterialStore::globalInstance();
        }

        const IPresetMaterialStore* constPresetMaterialStore()
        {
            return presetMaterialStore();
        }
    }
}
