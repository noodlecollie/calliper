#include "presetrendermodelmanager.h"

#include "rendersystem/private/stores/rendermodelstore/rendermodelstore.h"

#include "rendersystem/private/static-stores/presetrendermodelmanager/initialisers/screenspacequadrendermodelinitialiser.h"

namespace
{
    inline RenderSystem::RenderModelDefs::RenderModelId createPresetRenderModel(const IRenderModelInitialiser& initialiser)
    {
        RenderModelStore* store = RenderModelStore::globalInstance();
        Q_ASSERT_X(store, Q_FUNC_INFO, "Expected render model store to be initialised!");

        RenderSystem::RenderModelDefs::RenderModelId renderModelId = store->createRenderModel();
        Q_ASSERT_X(renderModelId != RenderSystem::RenderModelDefs::INVALID_OBJECT_ID, Q_FUNC_INFO, "Expected valid render model ID!");

        initialiser.initialise(renderModelId);

        return renderModelId;
    }

    PresetRenderModelManager::InitialiserFunction initialisers[] =
    {
        [](){ return createPresetRenderModel(ScreenSpaceQuadRenderModelInitialiser()); }
    };
}

PresetRenderModelManager::PresetRenderModelManager()
    : Containers::StaticObjectStoreArray<RenderSystem::RenderModelDefs::RenderModelId,
                                         PresetRenderModelManager::TOTAL_PRESET_RENDER_MODELS>()
{
    initialise(initialisers);
}

PresetRenderModelManager::~PresetRenderModelManager()
{
}
