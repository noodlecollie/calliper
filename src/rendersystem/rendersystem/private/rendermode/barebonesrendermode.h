#ifndef BAREBONESRENDERMODE_H
#define BAREBONESRENDERMODE_H

#include "baserendermode.h"

class BarebonesRenderMode : public BaseRenderMode
{
public:
    BarebonesRenderMode();

    virtual PrivateShaderDefs::ShaderId shaderId(RenderSystem::ShaderDefs::ShaderStyle style) const override;
};

#endif // BAREBONESRENDERMODE_H
