#ifndef UNLITPRESETMATERIALINITIALISER_H
#define UNLITPRESETMATERIALINITIALISER_H

#include "ipresetmaterialinitialiser.h"

#include "rendersystem/private/static-stores/presettexturestore/presettexturestore.h"
#include "rendersystem/private/stores/opengltexturestore/opengltexturestore.h"

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

        const QString path = PresetTextureStore::globalInstance()->path(UnlitPerVertexColorTexture);
        Q_ASSERT_X(!path.isEmpty(), Q_FUNC_INFO, "Expected valid path!");

        material->addTextureUnitMapping(RenderSystem::TextureDefs::MainTexture, path);
    }
};

#endif // UNLITPRESETMATERIALINITIALISER_H
