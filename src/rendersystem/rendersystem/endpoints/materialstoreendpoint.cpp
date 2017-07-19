#include "materialstoreendpoint.h"
#include "rendersystem/private/stores/materialstore/materialstore.h"

namespace RenderSystem
{
    namespace MaterialStoreEndpoint
    {
        IMaterialStore* materialStore()
        {
            return MaterialStore::globalInstance();
        }

        const IMaterialStore* constMaterialStore()
        {
            return materialStore();
        }
    }
}
