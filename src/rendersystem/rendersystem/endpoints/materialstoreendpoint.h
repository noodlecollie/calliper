#ifndef MATERIALSTOREENDPOINT_H
#define MATERIALSTOREENDPOINT_H

#include "rendersystem_global.h"
#include "rendersystem/interface-classes/currentcontextguard/currentcontextguard.h"
#include "rendersystem/interfaces/imaterialstore.h"

namespace RenderSystem
{
    namespace MaterialStoreEndpoint
    {
        typedef CurrentContextGuard<IMaterialStore> MaterialStoreAccessor;

        RENDERSYSTEMSHARED_EXPORT MaterialStoreAccessor materialStore();
    }
}

#endif // MATERIALSTOREENDPOINT_H
