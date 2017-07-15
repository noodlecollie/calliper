#ifndef RENDERMODESTORE_H
#define RENDERMODESTORE_H

#include "rendersystem/private/rendermode/baserendermode.h"
#include "containers/static/staticobjectstorearray.h"
#include "calliperutil/global/globalinstancehelper.h"

// These are plain shader pointers to indicate they should not be owned or stored by other objects.
class RenderModeStore : public Containers::StaticObjectStoreArray<BaseRenderMode*, RenderSystem::ShaderDefs::TOTAL_RENDER_MODES>,
                        public CalliperUtil::GlobalInstanceHelper<RenderModeStore>
{
public:
    RenderModeStore();
    virtual ~RenderModeStore();

private:
    typedef Containers::StaticObjectStoreArray<BaseRenderMode*, RenderSystem::ShaderDefs::TOTAL_RENDER_MODES> StoreType;
};

#endif // RENDERMODESTORE_H
