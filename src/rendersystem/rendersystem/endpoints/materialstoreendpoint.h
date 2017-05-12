#ifndef MATERIALSTOREENDPOINT_H
#define MATERIALSTOREENDPOINT_H

#include "rendersystem_global.h"
#include "rendersystem/interface-classes/currentcontextguard/currentcontextguard.h"
#include "rendersystem/interfaces/imaterialstore.h"

namespace RenderSystem
{
    RENDERSYSTEMSHARED_EXPORT CurrentContextGuard<IMaterialStore> materialStoreEndpoint();
};

#endif // MATERIALSTOREENDPOINT_H