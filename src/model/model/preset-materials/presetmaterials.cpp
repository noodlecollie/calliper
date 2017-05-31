#include "presetmaterials.h"

#include <functional>

#include "rendersystem/interface-classes/shader/publicshaderdefs.h"
#include "rendersystem/endpoints/materialstoreendpoint.h"

namespace PresetMaterials
{
    typedef QSharedPointer<RenderSystem::RenderMaterial> MaterialPointer;
    typedef std::function<void(const MaterialPointer&)> PresetMaterialInitialiser;
    typedef RenderSystem::PublicStoreDefs::MaterialId MaterialId;

    struct PresetMaterialInfo
    {
        MaterialId id;
        const QString name;
        const PresetMaterialInitialiser initialiser;

        PresetMaterialInfo(const QString& inName, PresetMaterialInitialiser inInitialiser)
            : id(RenderSystem::PublicStoreDefs::INVALID_MATERIAL_ID),
              name(inName),
              initialiser(inInitialiser)
        {
        }
    };

    PresetMaterialInfo g_PresetMaterials[] =
    {
        PresetMaterialInfo(
            "unlitPerVertexColor3D",
            [](const MaterialPointer& material) { material->setShaderStyle(RenderSystem::PublicShaderDefs::UnlitPerVertexColor3D); }
        ),
    };

    static_assert(sizeof(g_PresetMaterials)/sizeof(PresetMaterialInfo) == TOTAL_PRESET_MATERIALS,
                  "Preset material array is of unexpected length. Check all materials are catered for.");

    void initialisePresetMaterials()
    {
        using namespace RenderSystem;

        CurrentContextGuard<IMaterialStore> materialStore = MaterialStoreEndpoint::materialStore();

        for ( int i = 0; i < TOTAL_PRESET_MATERIALS; ++i )
        {
            PresetMaterialInfo& materialInfo = g_PresetMaterials[i];
            materialInfo.id = materialStore->createMaterial("_presetMaterials/" + materialInfo.name);
            materialInfo.initialiser(materialStore->material(materialInfo.id).toStrongRef());
        }
    }

    RenderSystem::PublicStoreDefs::MaterialId presetMaterialId(PresetMaterial presetMaterial)
    {
        if ( presetMaterial < 0 || presetMaterial >= TOTAL_PRESET_MATERIALS )
        {
            return RenderSystem::PublicStoreDefs::INVALID_MATERIAL_ID;
        }

        Q_ASSERT_X(g_PresetMaterials[presetMaterial].id != RenderSystem::PublicStoreDefs::INVALID_MATERIAL_ID,
                   Q_FUNC_INFO,
                   "Expected valid material ID. Has the collection been initialised?");

        return g_PresetMaterials[presetMaterial].id;
    }
}
