#ifndef RENDERMODESTOREKEY_H
#define RENDERMODESTOREKEY_H

#include "rendersystem/private/store-classes/staticobjectstorekey.h"
#include "rendersystem/interface-classes/definitions/publicshaderdefs.h"

class RenderModeStoreKey : public StaticObjectStoreKey<RenderSystem::PublicShaderDefs::RenderMode,
                                                       RenderSystem::PublicShaderDefs::TOTAL_RENDER_MODES>
{
};

#endif // RENDERMODESTOREKEY_H
