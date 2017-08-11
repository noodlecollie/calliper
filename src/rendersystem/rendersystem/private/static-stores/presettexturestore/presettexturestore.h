#ifndef PRESETTEXTURESTORE_H
#define PRESETTEXTURESTORE_H

#include "presettexturedefs.h"

#include "containers/static/staticobjectstorearray.h"
#include "calliperutil/global/globalinstancehelper.h"
#include "rendersystem/interface-classes/definitions/texturedefs.h"

class PresetTextureStore : public Containers::StaticObjectStoreArray<RenderSystem::TextureDefs::TextureId,
                                                                     TOTAL_PRESET_TEXTURES>,
                           public CalliperUtil::GlobalInstanceHelper<PresetTextureStore>
{
public:
    PresetTextureStore();
    virtual ~PresetTextureStore();

    RenderSystem::TextureDefs::TextureId texture(PresetTextureId presetId) const;
    QString path(PresetTextureId presetId) const;
};

#endif // PRESETTEXTURESTORE_H
