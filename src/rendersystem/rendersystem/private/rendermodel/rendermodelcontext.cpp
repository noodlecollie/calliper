#include "rendermodelcontext.h"

RenderModelContext::RenderModelContext()
    : m_nRenderMode(RenderSystem::PublicShaderDefs::UnknownRenderMode)
{

}

RenderSystem::PublicShaderDefs::RenderMode RenderModelContext::renderMode() const
{
    return m_nRenderMode;
}

void RenderModelContext::setRenderMode(RenderSystem::PublicShaderDefs::RenderMode renderMode)
{
    m_nRenderMode = renderMode;
}
