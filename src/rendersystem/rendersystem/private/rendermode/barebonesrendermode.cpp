#include "barebonesrendermode.h"

BarebonesRenderMode::BarebonesRenderMode()
    : BaseRenderMode(RenderSystem::ShaderDefs::BarebonesRenderMode)
{

}

RenderSystem::ShaderDefs::ShaderId BarebonesRenderMode::shaderId(RenderSystem::ShaderDefs::ShaderId originalId) const
{
    // TODO
    return BaseRenderMode::shaderId(originalId);
}
