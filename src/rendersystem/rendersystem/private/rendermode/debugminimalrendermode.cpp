#include "debugminimalrendermode.h"

DebugMinimalRenderMode::DebugMinimalRenderMode()
    : BaseRenderMode(RenderSystem::ShaderDefs::DebugMinimalMode)
{

}

PrivateShaderDefs::ShaderId DebugMinimalRenderMode::shaderId(RenderSystem::ShaderDefs::ShaderStyle style) const
{
    Q_UNUSED(style);
    return PrivateShaderDefs::DebugMinimalShaderId;
}
