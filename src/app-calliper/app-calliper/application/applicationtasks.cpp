#include "applicationtasks.h"

#include "calliperutil/debug/debug.h"

#include "rendersystem/global/rendersystem.h"

namespace ApplicationTasks
{
    void initSubSystems()
    {
        RenderSystem::Global::initialise();
    }

    void shutDownSubSystems()
    {
        RenderSystem::Global::shutdown();
    }
}
