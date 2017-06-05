#include "baserendermode.h"

BaseRenderMode::BaseRenderMode(RenderSystem::ShaderDefs::RenderMode renderMode)
    : m_nRenderMode(renderMode)
{

}

BaseRenderMode::~BaseRenderMode()
{

}

RenderSystem::ShaderDefs::RenderMode BaseRenderMode::renderMode() const
{
    return m_nRenderMode;
}

PrivateShaderDefs::ShaderId BaseRenderMode::shaderId(RenderSystem::ShaderDefs::ShaderStyle style) const
{
    Q_UNUSED(style);
    return PrivateShaderDefs::ErrorShaderId;
}
