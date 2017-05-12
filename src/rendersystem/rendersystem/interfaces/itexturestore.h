#ifndef ITEXTURESTORE_H
#define ITEXTURESTORE_H

#include "rendersystem_global.h"

#include <QImage>

#include "rendersystem/interface-classes/store-defs/publicstoredefs.h"

namespace RenderSystem
{
    class ITextureStore
    {
    public:
        virtual ~ITextureStore() {}

        // Adds a texture to the store, using the provided image.
        // The texture is assigned the given path.
        // Returns the ID of the texture.
        virtual PublicStoreDefs::TextureId addTexture(const QImage& image, const QString& path) = 0;

        // Removes the texture with the given ID from the store.
        // If a texture with this ID does not exist, this function does nothing.
        virtual void removeTexture(const PublicStoreDefs::TextureId id) = 0;

        // Returns whether a texture with the given ID exists in the store.
        virtual bool containsTexture(const PublicStoreDefs::TextureId id) const = 0;

        // Returns whether a texture with the given path exists in the store.
        virtual bool containsTexture(const QString& strPath) const = 0;

        // Returns the ID for the texture with the given path, or 0 if it doesn't exist.
        virtual PublicStoreDefs::TextureId textureIdFromPath(const QString& path) const = 0;

        // Returns the path for the texture with the given ID, or 0 if it doesn't exist.
        virtual QString texturePathFromId(const PublicStoreDefs::TextureId id) const = 0;
    };
};

#endif // ITEXTURESTORE_H
