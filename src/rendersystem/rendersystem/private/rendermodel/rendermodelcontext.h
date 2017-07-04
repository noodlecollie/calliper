#ifndef RENDERMODELCONTEXT_H
#define RENDERMODELCONTEXT_H

#include <QOpenGLFunctions>

#include "rendersystem/interface-classes/definitions/shaderdefs.h"

class RenderModelContext
{
public:
    RenderModelContext();

    GLint uniformBufferOffsetAlignment() const;

    RenderSystem::ShaderDefs::RenderMode renderMode() const;
    void setRenderMode(RenderSystem::ShaderDefs::RenderMode renderMode);

private:
    void getOpenGLAttributes();

    GLint m_nUniformBufferOffsetAlignment;

    RenderSystem::ShaderDefs::RenderMode m_nRenderMode;
};

#endif // RENDERMODELCONTEXT_H
