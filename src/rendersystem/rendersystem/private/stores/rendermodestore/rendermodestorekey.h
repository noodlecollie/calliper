#ifndef RENDERMODESTOREKEY_H
#define RENDERMODESTOREKEY_H

#include "rendersystem/private/store-classes/staticobjectstorekey.h"
#include "rendersystem/interface-classes/definitions/shaderdefs.h"

class RenderModeStoreKey : public StaticObjectStoreKey<RenderSystem::ShaderDefs::RenderMode,
                                                       RenderSystem::ShaderDefs::TOTAL_RENDER_MODES>
{
};

#endif // RENDERMODESTOREKEY_H
