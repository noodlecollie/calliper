#include "presetmaterialmanager.h"

#include "rendersystem/private/stores/materialstore/materialstore.h"

#include "initialisers/unlitpresetmaterialinitialiser.h"

namespace
{
    inline RenderSystem::MaterialDefs::MaterialId createPresetMaterial(const IPresetMaterialInitialiser& initialiser)
    {
        MaterialStore* store = MaterialStore::globalInstance();
        Q_ASSERT_X(store, Q_FUNC_INFO, "Expected material store to be initialised!");

        RenderSystem::MaterialDefs::MaterialId materialId = store->createMaterial("_presetMaterials/" + initialiser.name());
        Q_ASSERT_X(materialId != RenderSystem::MaterialDefs::INVALID_MATERIAL_ID, Q_FUNC_INFO, "Expected valid material ID!");

        QSharedPointer<RenderSystem::RenderMaterial> material = store->material(materialId).toStrongRef();
        Q_ASSERT_X(material, Q_FUNC_INFO, "Expected valid material!");

        initialiser.initialise(material);

        return materialId;
    }

    PresetMaterialManager::InitialiserFunction initialisers[] =
    {
        [](){ return createPresetMaterial(UnlitPresetMaterialInitialiser()); }
    };
}

PresetMaterialManager::PresetMaterialManager()
    : Containers::StaticObjectStoreArray<RenderSystem::MaterialDefs::MaterialId,
                                         RenderSystem::MaterialDefs::TOTAL_PRESET_MATERIALS>()
{
    initialise(initialisers);
}

PresetMaterialManager::~PresetMaterialManager()
{
}

RenderSystem::MaterialDefs::MaterialId PresetMaterialManager::material(const RenderSystem::MaterialDefs::PresetMaterial presetMaterial) const
{
    return object(presetMaterial);
}
