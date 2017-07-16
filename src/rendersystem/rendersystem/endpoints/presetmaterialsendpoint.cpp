#include "presetmaterialsendpoint.h"
#include "rendersystem/private/static-stores/presetmaterialmanager/presetmaterialmanager.h"

namespace RenderSystem
{
    namespace PresetMaterialsEndpoint
    {
        PresetMaterialsAccessor presetMaterials()
        {
            return PresetMaterialsAccessor(*PresetMaterialManager::globalInstance());
        }
    }
}
