#ifndef PRESETMATERIALSTORE_H
#define PRESETMATERIALSTORE_H

#include "rendersystem/interfaces/ipresetmaterials.h"

#include "containers/static/staticobjectstorearray.h"

#include "calliperutil/global/globalinstancehelper.h"

class PresetMaterialStore : public Containers::StaticObjectStoreArray<RenderSystem::MaterialDefs::MaterialId,
                                                                        RenderSystem::MaterialDefs::TOTAL_PRESET_MATERIALS>,
                              public RenderSystem::IPresetMaterials,
                              public CalliperUtil::GlobalInstanceHelper<PresetMaterialStore>
{
public:
    PresetMaterialStore();
    virtual ~PresetMaterialStore();

    // External
    virtual RenderSystem::MaterialDefs::MaterialId material(const RenderSystem::MaterialDefs::PresetMaterial presetMaterial) const override;
};

#endif // PRESETMATERIALSTORE_H
