#include "rendermodelcontext.h"

#include "calliperutil/opengl/openglhelpers.h"
#include "calliperutil/opengl/openglerrors.h"

RenderModelContext::RenderModelContext()
    : m_nUniformBufferOffsetAlignment(0),
      m_nUniformBlockDataSize(0),
      m_nRenderMode(RenderSystem::ShaderDefs::UnknownRenderMode)
{
    getOpenGLAttributes();
}

void RenderModelContext::getOpenGLAttributes()
{
    // See https://www.opengl.org/discussion_boards/showthread.php/175410-Uniform-Buffers-and-BindBufferRange
    GL_CURRENT_F;

    GLTRY(f->glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &m_nUniformBufferOffsetAlignment));
    GLTRY(f->glGetIntegerv(GL_UNIFORM_BLOCK_DATA_SIZE, &m_nUniformBlockDataSize));
}

GLint RenderModelContext::uniformBufferOffsetAlignment() const
{
    return m_nUniformBufferOffsetAlignment;
}

GLint RenderModelContext::uniformBlockDataSize() const
{
    return m_nUniformBufferOffsetAlignment;
}

RenderSystem::ShaderDefs::RenderMode RenderModelContext::renderMode() const
{
    return m_nRenderMode;
}

void RenderModelContext::setRenderMode(RenderSystem::ShaderDefs::RenderMode renderMode)
{
    m_nRenderMode = renderMode;
}
