#ifndef VTFLOADER_H
#define VTFLOADER_H

#include "model-loaders_global.h"
#include "vpk/vpkfilecollection.h"
#include "stores/materialstore.h"
#include "stores/texturestore.h"

namespace ModelLoaders
{
    namespace VTFLoader
    {
        MODELLOADERSSHARED_EXPORT void loadMaterials(const FileFormats::VPKFileCollection& vpkFiles,
                                                     Model::MaterialStore* materialStore,
                                                     Model::TextureStore* textureStore);

        MODELLOADERSSHARED_EXPORT void debugVmtLoading();
    }
}

#endif // VTFLOADER_H
