#ifndef SCREENSPACEQUADRENDERMODELINITIALISER_H
#define SCREENSPACEQUADRENDERMODELINITIALISER_H

#include "irendermodelinitialiser.h"

class ScreenSpaceQuadRenderModelInitialiser : public IRenderModelInitialiser
{
public:
    virtual void initialise(RenderSystem::RenderModelDefs::RenderModelId renderModelId) const override
    {
        using namespace RenderSystem;

        // TODO: Add geometry.
        // This will require that we create a way within the materials
        // to specify using a frame buffer as a texture.
    }
};

#endif // SCREENSPACEQUADRENDERMODELINITIALISER_H
