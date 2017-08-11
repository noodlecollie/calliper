#include "presettexturestore.h"

#include <QFileInfo>

#include "rendersystem/private/stores/opengltexturestore/opengltexturestore.h"
#include "calliperutil/array/arrayutil.h"

namespace
{
    static const QString texturePaths[] =
    {
        "_internal/white.png"
    };

    inline QString cleanedTexturePath(const QString& path)
    {
         return QFileInfo(path).filePath();
    }

    inline RenderSystem::TextureDefs::TextureId createPresetTexture(PresetTextureId id)
    {
        OpenGLTextureStore* const store = OpenGLTextureStore::globalInstance();

        const QString& path = texturePaths[id];
        RenderSystem::TextureDefs::TextureId textureId = store->addTexture(QImage(":/textures/" + path), cleanedTexturePath(path));
        Q_ASSERT_X(textureId != RenderSystem::TextureDefs::INVALID_TEXTURE_ID, Q_FUNC_INFO, "Expected valid texture ID!");

        return textureId;
    }

    PresetTextureStore::InitialiserFunction initialisers[] =
    {
        [](){ return createPresetTexture(UnlitPerVertexColorTexture); },
    };
}

PresetTextureStore::PresetTextureStore()
    : Containers::StaticObjectStoreArray<RenderSystem::TextureDefs::TextureId,
                                         TOTAL_PRESET_TEXTURES>()
{
    static_assert(SIZEOF_ARRAY(texturePaths) == TOTAL_PRESET_TEXTURES,
                  "Size of texture path array does not match expected size. "
                  "Do you need to add a new preset texture ID?");

    initialise(initialisers);
}

PresetTextureStore::~PresetTextureStore()
{

}

RenderSystem::TextureDefs::TextureId PresetTextureStore::texture(PresetTextureId presetId) const
{
    return object(presetId);
}

QString PresetTextureStore::path(PresetTextureId presetId) const
{
    return OpenGLTextureStore::globalInstance()->texturePathFromId(texture(presetId));
}
