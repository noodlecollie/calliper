#ifndef BAREBONESRENDERMODE_H
#define BAREBONESRENDERMODE_H

#include "baserendermode.h"

class BarebonesRenderMode : public BaseRenderMode
{
public:
    BarebonesRenderMode();

    virtual RenderSystem::ShaderDefs::ShaderId shaderId(RenderSystem::ShaderDefs::ShaderId originalId) const override;
};

#endif // BAREBONESRENDERMODE_H
