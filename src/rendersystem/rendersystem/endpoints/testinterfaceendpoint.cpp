#include "testinterfaceendpoint.h"
#include "rendersystem/private/test/testinterface.h"

namespace
{
    RenderSystem::TestInterface localTestInterface;
}

namespace RenderSystem
{
    CurrentContextGuard<ITestInterface> testInterfaceEndpoint()
    {
        return CurrentContextGuard<ITestInterface>(localTestInterface);
    }
}
