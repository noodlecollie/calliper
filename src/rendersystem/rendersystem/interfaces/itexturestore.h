#ifndef ITEXTURESTORE_H
#define ITEXTURESTORE_H

#include "rendersystem_global.h"

#include <QImage>

#include "rendersystem/interface-classes/definitions/texturedefs.h"
#include "rendersystem/interface-classes/texture/namedopengltexture.h"

namespace RenderSystem
{
    class ITextureStore
    {
    public:
        virtual ~ITextureStore() {}

        // Adds a texture to the store, using the provided image.
        // The texture is assigned the given path.
        // Returns the ID of the texture.
        virtual TextureDefs::TextureId addTexture(const QImage& image, const QString& path) = 0;

        // Creates a texture with the given path. The texture begins uninitialised.
        // Returns the ID of the texture.
        virtual TextureDefs::TextureId createBlankTexture(const QString& path) = 0;

        // Removes the texture with the given ID from the store.
        // If a texture with this ID does not exist, this function does nothing.
        virtual void removeTexture(const TextureDefs::TextureId id) = 0;

        // Returns whether a texture with the given ID exists in the store.
        virtual bool containsTexture(const TextureDefs::TextureId id) const = 0;

        // Returns whether a texture with the given path exists in the store.
        virtual bool containsTexture(const QString& strPath) const = 0;

        // Returns the ID for the texture with the given path, or 0 if it doesn't exist.
        virtual TextureDefs::TextureId textureIdFromPath(const QString& path) const = 0;

        // Returns the path for the texture with the given ID, or 0 if it doesn't exist.
        virtual QString texturePathFromId(const TextureDefs::TextureId id) const = 0;

        // Returns a pointer to the texture with the given ID.
        // If no texture exists with this ID, the default texture is returned.
        // Weak pointers are returned to indicate that the store still retains
        // full ownership of the material - don't store QSharedPointers.
        // The weak pointer is, however, guaranteed to be valid immediately after the call.
        virtual QWeakPointer<NamedOpenGLTexture> texture(const TextureDefs::TextureId textureId) const = 0;
        virtual QWeakPointer<NamedOpenGLTexture> texture(const QString& path) const = 0;
    };
}

#endif // ITEXTURESTORE_H
