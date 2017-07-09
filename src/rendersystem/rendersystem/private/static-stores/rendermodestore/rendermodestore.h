#ifndef RENDERMODESTORE_H
#define RENDERMODESTORE_H

#include "rendermodestorekey.h"

#include "rendersystem/private/rendermode/baserendermode.h"

#include "containers/static/staticobjectstore.h"

#include "calliperutil/global/globalinstancehelper.h"

// These are plain shader pointers to indicate they should not be owned or stored by other objects.
class RenderModeStore : public Containers::StaticObjectStore<BaseRenderMode*,
                                                             RenderModeStoreKey>,
                        public CalliperUtil::GlobalInstanceHelper<RenderModeStore>
{
public:
    RenderModeStore();

protected:
    virtual void onStoreInitialised() override;
    virtual void onStoreDestroyed() override;

private:
    static RenderModeStore* m_pGlobalInstance;
};

#endif // RENDERMODESTORE_H
