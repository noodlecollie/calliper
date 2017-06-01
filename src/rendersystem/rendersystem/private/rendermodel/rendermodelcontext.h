#ifndef RENDERMODELCONTEXT_H
#define RENDERMODELCONTEXT_H

#include "rendersystem/interface-classes/definitions/publicshaderdefs.h"

class RenderModelContext
{
public:
    RenderModelContext();

    RenderSystem::PublicShaderDefs::RenderMode renderMode() const;
    void setRenderMode(RenderSystem::PublicShaderDefs::RenderMode renderMode);

private:
    RenderSystem::PublicShaderDefs::RenderMode m_nRenderMode;
};

#endif // RENDERMODELCONTEXT_H
