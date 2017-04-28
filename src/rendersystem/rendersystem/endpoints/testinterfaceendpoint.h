#ifndef TESTINTERFACEENDPOINT_H
#define TESTINTERFACEENDPOINT_H

#include "rendersystem_global.h"
#include "rendersystem/interface-classes/currentcontextguard/currentcontextguard.h"
#include "rendersystem/interfaces/itestinterface.h"

namespace RenderSystem
{
    RENDERSYSTEMSHARED_EXPORT CurrentContextGuard<ITestInterface> testInterfaceEndpoint();
}

#endif // TESTINTERFACEENDPOINT_H
