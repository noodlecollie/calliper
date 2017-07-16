#ifndef PRESETMATERIALMANAGER_H
#define PRESETMATERIALMANAGER_H

#include "rendersystem/interfaces/ipresetmaterials.h"

#include "containers/static/staticobjectstorearray.h"

#include "calliperutil/global/globalinstancehelper.h"

class PresetMaterialManager : public Containers::StaticObjectStoreArray<RenderSystem::MaterialDefs::MaterialId,
                                                                        RenderSystem::MaterialDefs::TOTAL_PRESET_MATERIALS>,
                              public RenderSystem::IPresetMaterials,
                              public CalliperUtil::GlobalInstanceHelper<PresetMaterialManager>
{
public:
    PresetMaterialManager();
    virtual ~PresetMaterialManager();

    // External
    virtual RenderSystem::MaterialDefs::MaterialId material(const RenderSystem::MaterialDefs::PresetMaterial presetMaterial) const override;
};

#endif // PRESETMATERIALMANAGER_H
