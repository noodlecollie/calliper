#include "rendermodelcontext.h"

#include "calliperutil/opengl/openglhelpers.h"
#include "calliperutil/opengl/openglerrors.h"

RenderModelContext::RenderModelContext()
    : m_nRenderMode(RenderSystem::ShaderDefs::UnknownRenderMode)
{
}

RenderSystem::ShaderDefs::RenderMode RenderModelContext::renderMode() const
{
    return m_nRenderMode;
}

void RenderModelContext::setRenderMode(RenderSystem::ShaderDefs::RenderMode renderMode)
{
    m_nRenderMode = renderMode;
}
