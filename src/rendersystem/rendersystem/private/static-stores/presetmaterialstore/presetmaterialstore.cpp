#include "presetmaterialstore.h"

#include "rendersystem/private/stores/materialstore/materialstore.h"

#include "initialisers/unlitpresetmaterialinitialiser.h"

namespace
{
    inline RenderSystem::MaterialDefs::MaterialId createPresetMaterial(const IPresetMaterialInitialiser& initialiser)
    {
        MaterialStore* const store = MaterialStore::globalInstance();

        RenderSystem::MaterialDefs::MaterialId materialId = store->createMaterial("_presetMaterials/" + initialiser.name());
        Q_ASSERT_X(materialId != RenderSystem::MaterialDefs::INVALID_MATERIAL_ID, Q_FUNC_INFO, "Expected valid material ID!");

        QSharedPointer<RenderSystem::RenderMaterial> material = store->material(materialId).toStrongRef();
        Q_ASSERT_X(material, Q_FUNC_INFO, "Expected valid material!");

        initialiser.initialise(material);

        return materialId;
    }

    PresetMaterialStore::InitialiserFunction initialisers[] =
    {
        [](){ return createPresetMaterial(UnlitPresetMaterialInitialiser()); }
    };
}

PresetMaterialStore::PresetMaterialStore()
    : Containers::StaticObjectStoreArray<RenderSystem::MaterialDefs::MaterialId,
                                         RenderSystem::MaterialDefs::TOTAL_PRESET_MATERIALS>()
{
    initialise(initialisers);
}

PresetMaterialStore::~PresetMaterialStore()
{
}

RenderSystem::MaterialDefs::MaterialId PresetMaterialStore::material(const RenderSystem::MaterialDefs::PresetMaterial presetMaterial) const
{
    return object(presetMaterial);
}
