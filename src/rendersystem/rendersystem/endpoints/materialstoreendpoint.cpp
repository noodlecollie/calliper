#include "materialstoreendpoint.h"
#include "rendersystem/private/stores/materialstore/materialstore.h"


namespace
{
    MaterialStore localMaterialStore;
}

namespace RenderSystem
{
    namespace MaterialStoreEndpoint
    {
        CurrentContextGuard<IMaterialStore> materialStore()
        {
            return CurrentContextGuard<IMaterialStore>(localMaterialStore);
        }
    }
}
