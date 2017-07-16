#ifndef IRENDERMODELINITIALISER_H
#define IRENDERMODELINITIALISER_H

#include "rendersystem/interface-classes/definitions/rendermodeldefs.h"

class IRenderModelInitialiser
{
public:
    virtual ~IRenderModelInitialiser() {}

    virtual void initialise(RenderSystem::RenderModelDefs::RenderModelId renderModelId) const = 0;
}

#endif // IRENDERMODELINITIALISER_H
