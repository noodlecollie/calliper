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

RenderSystem::ShaderDefs::ShaderId BaseRenderMode::shaderId(RenderSystem::ShaderDefs::ShaderId originalId) const
{
    // By default don't override the shader.
    return originalId;
}
