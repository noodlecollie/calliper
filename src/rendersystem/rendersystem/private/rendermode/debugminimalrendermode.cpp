#include "debugminimalrendermode.h"

DebugMinimalRenderMode::DebugMinimalRenderMode()
    : BaseRenderMode(RenderSystem::ShaderDefs::DebugMinimalMode)
{

}

RenderSystem::ShaderDefs::ShaderId DebugMinimalRenderMode::shaderId(RenderSystem::ShaderDefs::ShaderId originalId) const
{
    Q_UNUSED(originalId);

    return RenderSystem::ShaderDefs::DebugMinimalShaderId;
}
