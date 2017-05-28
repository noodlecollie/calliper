#ifndef RENDERMODESTORE_H
#define RENDERMODESTORE_H

#include "rendermodestorekey.h"

#include "rendersystem/private/store-classes/staticobjectstore.h"
#include "rendersystem/private/rendermode/baserendermode.h"
#include "rendersystem/private/store-classes/globalinstancehelper.h"

// These are plain shader pointers to indicate they should not be owned or stored by other objects.
class RenderModeStore : public StaticObjectStore<BaseRenderMode*,
                                                 RenderModeStoreKey>,
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
