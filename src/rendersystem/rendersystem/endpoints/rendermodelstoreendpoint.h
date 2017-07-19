#ifndef RENDERMODELSTOREENDPOINT_H
#define RENDERMODELSTOREENDPOINT_H

#include "rendersystem_global.h"
#include "rendersystem/interface-classes/currentcontextguard/currentcontextguard.h"
#include "rendersystem/interfaces/irendermodelstore.h"

namespace RenderSystem
{
    namespace RenderModelStoreEndpoint
    {
        typedef CurrentContextGuard<IRenderModelStore> RenderModelStoreAccessor;

        RENDERSYSTEMSHARED_EXPORT RenderModelStoreAccessor renderModelStore();
        RENDERSYSTEMSHARED_EXPORT const IRenderModelStore* constRenderModelStore();
    }
}

#endif // RENDERMODELSTOREENDPOINT_H
