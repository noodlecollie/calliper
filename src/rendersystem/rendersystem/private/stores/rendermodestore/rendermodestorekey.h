#ifndef RENDERMODESTOREKEY_H
#define RENDERMODESTOREKEY_H

#include "containers/static/staticobjectstorekey.h"
#include "rendersystem/interface-classes/definitions/shaderdefs.h"

class RenderModeStoreKey : public Containers::StaticObjectStoreKey<RenderSystem::ShaderDefs::RenderMode,
                                                                   RenderSystem::ShaderDefs::TOTAL_RENDER_MODES>
{
};

#endif // RENDERMODESTOREKEY_H
