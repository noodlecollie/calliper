#include "materialstoreendpoint.h"
#include "rendersystem/private/stores/materialstore/materialstore.h"

namespace RenderSystem
{
    namespace MaterialStoreEndpoint
    {
        CurrentContextGuard<IMaterialStore> materialStore()
        {
            return CurrentContextGuard<IMaterialStore>(*MaterialStore::globalInstance());
        }
    }
}
