#ifndef PRESETRENDERMODELMANAGER_H
#define PRESETRENDERMODELMANAGER_H

#include "rendersystem/interface-classes/definitions/rendermodeldefs.h"

#include "containers/static/staticobjectstorearray.h"

#include "calliperutil/global/globalinstancehelper.h"

class PresetRenderModelManager : public Containers::StaticObjectStoreArray<RenderSystem::RenderModelDefs::RenderModelId,
                                                                           RenderSystem::RenderModelDefs::TOTAL_PRESET_RENDER_MODELS>,
                                 public CalliperUtil::GlobalInstanceHelper<PresetRenderModelManager>
{
public:
    PresetRenderModelManager();
    virtual ~PresetRenderModelManager();
};

#endif // PRESETRENDERMODELMANAGER_H
