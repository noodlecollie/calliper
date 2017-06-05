#ifndef BASERENDERMODE_H
#define BASERENDERMODE_H

#include "rendersystem/interface-classes/definitions/shaderdefs.h"
#include "rendersystem/private/shaders/common/privateshaderdefs.h"

class BaseRenderMode
{
public:
    BaseRenderMode(RenderSystem::ShaderDefs::RenderMode renderMode);
    virtual ~BaseRenderMode();

    RenderSystem::ShaderDefs::RenderMode renderMode() const;
    virtual PrivateShaderDefs::ShaderId shaderId(RenderSystem::ShaderDefs::ShaderStyle style) const;

private:
    const RenderSystem::ShaderDefs::RenderMode m_nRenderMode;
};

#endif // BASERENDERMODE_H
