#ifndef RENDERMODESTORE_H
#define RENDERMODESTORE_H

#include "rendersystem/private/store-classes/staticobjectstore.h"
#include "rendersystem/private/rendermode/baserendermode.h"
#include "rendersystem/private/shaders/common/privateshaderdefs.h"
#include "rendersystem/private/store-classes/globalinstancehelper.h"

// These are plain shader pointers to indicate they should not be owned or stored by other objects.
class RenderModeStore : public StaticObjectStore<BaseRenderMode*,
                                                 RenderSystem::PublicShaderDefs::RenderMode,
                                                 RenderSystem::PublicShaderDefs::TOTAL_RENDER_MODES>,
                        public GlobalInstanceHelper<RenderModeStore>
{
public:
    RenderModeStore();

protected:
    virtual void storeInitialised() override;
    virtual void storeDestroyed() override;

private:
    static RenderModeStore* m_pGlobalInstance;
};

#endif // RENDERMODESTORE_H
