#include "materialstoreendpoint.h"
#include "rendersystem/private/stores/materialstore/materialstore.h"


namespace
{
    MaterialStore localMaterialStore;
}

namespace RenderSystem
{
    CurrentContextGuard<IMaterialStore> materialStoreEndpoint()
    {
        return CurrentContextGuard<IMaterialStore>(localMaterialStore);
    }
}
