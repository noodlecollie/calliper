#ifndef ITEXTURESTORE_H
#define ITEXTURESTORE_H

#include "rendersystem_global.h"

#include <QImage>

namespace RenderSystem
{
    class ITextureStore
    {
    public:
        // A texture ID of 0 is invalid.
        typedef quint32 TextureId;

        virtual ~ITextureStore() {}

        // Adds a texture to the store, using the provided image.
        // The texture is assigned the given path.
        // Returns the ID of the texture.
        quint32 addTexture(const QImage& image, const QString& path) = 0;

        // Removes the texture with the given ID from the store.
        // If a texture with this ID does not exist, this function does nothing.
        void removeTexture(const TextureId id) = 0;

        // Returns whether a texture with the given ID exists in the store.
        bool containsTexture(const TextureId id) const = 0;

        // Returns the ID for the texture with the given path, or 0 if it doesn't exist.
        TextureId textureIdFromPath(const QString& path) const = 0;

        // Returns the path for the texture with the given ID, or 0 if it doesn't exist.
        QString texturePathFromId(const TextureId id) const = 0;
    };
};

#endif // ITEXTURESTORE_H
