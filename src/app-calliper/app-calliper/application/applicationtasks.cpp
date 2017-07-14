#include "applicationtasks.h"

#include "calliperutil/debug/debug.h"

#include "rendersystem/global/rendersystem.h"
#include "model/preset-materials/presetmaterials.h"

namespace AppCalliper
{
    namespace ApplicationTasks
    {
        void initSubSystems()
        {
            RenderSystem::Global::initialise();
            PresetMaterials::initialisePresetMaterials();
        }

        void shutDownSubSystems()
        {
            RenderSystem::Global::shutdown();
        }
    }
}
