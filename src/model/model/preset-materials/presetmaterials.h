#ifndef PRESETMATERIALS_H
#define PRESETMATERIALS_H

#include "model_global.h"
#include "rendersystem/endpoints/materialstoreendpoint.h"

namespace PresetMaterials
{
    enum PresetMaterial
    {
        UnlitPerVertexColor3D = 0,

        TOTAL_PRESET_MATERIALS
    };

    MODELSHARED_EXPORT void initialisePresetMaterials();
    RenderSystem::MaterialDefs::MaterialId presetMaterialId(PresetMaterial presetMaterial);
}

#endif // PRESETMATERIALS_H
