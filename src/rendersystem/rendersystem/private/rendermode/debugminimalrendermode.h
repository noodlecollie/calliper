#ifndef DEBUGMINIMALRENDERMODE_H
#define DEBUGMINIMALRENDERMODE_H

#include "baserendermode.h"

class DebugMinimalRenderMode : public BaseRenderMode
{
public:
    DebugMinimalRenderMode();

    virtual PrivateShaderDefs::ShaderId shaderId(RenderSystem::ShaderDefs::ShaderStyle style) const override;
};

#endif // DEBUGMINIMALRENDERMODE_H
