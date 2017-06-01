#include "materialstoreendpoint.h"
#include "rendersystem/private/stores/materialstore/materialstore.h"

namespace RenderSystem
{
    namespace MaterialStoreEndpoint
    {
        MaterialStoreAccessor materialStore()
        {
            return MaterialStoreAccessor(*MaterialStore::globalInstance());
        }
    }
}
