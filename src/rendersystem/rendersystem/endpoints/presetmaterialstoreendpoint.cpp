#include "presetmaterialstoreendpoint.h"
#include "rendersystem/private/static-stores/presetmaterialstore/presetmaterialstore.h"

namespace RenderSystem
{
    namespace PresetMaterialStoreEndpoint
    {
        PresetMaterialStoreAccessor presetMaterialStore()
        {
            return PresetMaterialStoreAccessor(*PresetMaterialStore::globalInstance());
        }
    }
}
