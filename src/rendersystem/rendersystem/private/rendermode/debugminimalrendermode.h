#ifndef DEBUGMINIMALRENDERMODE_H
#define DEBUGMINIMALRENDERMODE_H

#include "baserendermode.h"

class DebugMinimalRenderMode : public BaseRenderMode
{
public:
    DebugMinimalRenderMode();

    virtual RenderSystem::ShaderDefs::ShaderId shaderId(RenderSystem::ShaderDefs::ShaderId originalId) const override;
};

#endif // DEBUGMINIMALRENDERMODE_H
