#ifndef MATERIALSTOREENDPOINT_H
#define MATERIALSTOREENDPOINT_H

#include "rendersystem_global.h"
#include "rendersystem/interface-classes/currentcontextguard/currentcontextguard.h"
#include "rendersystem/interfaces/imaterialstore.h"

namespace RenderSystem
{
    namespace MaterialStoreEndpoint
    {
        RENDERSYSTEMSHARED_EXPORT CurrentContextGuard<IMaterialStore> materialStore();
    }
};

#endif // MATERIALSTOREENDPOINT_H
