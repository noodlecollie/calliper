#ifndef BASERENDERMODE_H
#define BASERENDERMODE_H

#include "rendersystem/interface-classes/shader/publicshaderdefs.h"
#include "rendersystem/private/shaders/common/privateshaderdefs.h"

class BaseRenderMode
{
public:
    BaseRenderMode(RenderSystem::PublicShaderDefs::RenderMode renderMode);
    virtual ~BaseRenderMode();

    RenderSystem::PublicShaderDefs::RenderMode renderMode() const;
    virtual PrivateShaderDefs::ShaderId shaderId(RenderSystem::PublicShaderDefs::ShaderStyle style) const = 0;

private:
    const RenderSystem::PublicShaderDefs::RenderMode m_nRenderMode;
};

#endif // BASERENDERMODE_H
