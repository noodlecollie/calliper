#ifndef RENDERUTILS_H
#define RENDERUTILS_H

#include "rendersystem/interface-classes/definitions/materialdefs.h"
#include "rendersystem/interface-classes/definitions/shaderdefs.h"
#include "rendersystem/private/shaders/common/privateshaderdefs.h"

namespace RenderUtils
{
    RenderSystem::ShaderDefs::ShaderId shaderFromMaterial(RenderSystem::ShaderDefs::RenderMode renderMode,
                                                          RenderSystem::MaterialDefs::MaterialId materialId);
}

#endif // RENDERUTILS_H
