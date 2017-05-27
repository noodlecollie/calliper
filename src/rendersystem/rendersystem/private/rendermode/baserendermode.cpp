#include "baserendermode.h"

BaseRenderMode::BaseRenderMode(RenderSystem::PublicShaderDefs::RenderMode renderMode)
    : m_nRenderMode(renderMode)
{

}

BaseRenderMode::~BaseRenderMode()
{

}

RenderSystem::PublicShaderDefs::RenderMode BaseRenderMode::renderMode() const
{
    return m_nRenderMode;
}
