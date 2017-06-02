#include "rendermodelstoreendpoint.h"
#include "rendersystem/private/stores/rendermodelstore/rendermodelstore.h"

namespace RenderSystem
{
    namespace RenderModelStoreEndpoint
    {
        RenderModelStoreAccessor renderModelStore()
        {
            return RenderModelStoreAccessor(*RenderModelStore::globalInstance());
        }
    }
}
