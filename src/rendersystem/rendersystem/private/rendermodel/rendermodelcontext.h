#ifndef RENDERMODELCONTEXT_H
#define RENDERMODELCONTEXT_H

#include "rendersystem/interface-classes/definitions/shaderdefs.h"

class RenderModelContext
{
public:
    RenderModelContext();

    RenderSystem::ShaderDefs::RenderMode renderMode() const;
    void setRenderMode(RenderSystem::ShaderDefs::RenderMode renderMode);

private:
    RenderSystem::ShaderDefs::RenderMode m_nRenderMode;
};

#endif // RENDERMODELCONTEXT_H
