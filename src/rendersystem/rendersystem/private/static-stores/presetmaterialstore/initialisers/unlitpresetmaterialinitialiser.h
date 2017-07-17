#ifndef UNLITPRESETMATERIALINITIALISER_H
#define UNLITPRESETMATERIALINITIALISER_H

#include "ipresetmaterialinitialiser.h"

class UnlitPresetMaterialInitialiser : public IPresetMaterialInitialiser
{
public:
    virtual QString name() const override
    {
        return "unlitPerVertexColor3D";
    }

    virtual void initialise(const QSharedPointer<RenderSystem::RenderMaterial> &material) const override
    {
        material->setShaderId(RenderSystem::ShaderDefs::UnlitGenericShaderId);
    }
};

#endif // UNLITPRESETMATERIALINITIALISER_H
