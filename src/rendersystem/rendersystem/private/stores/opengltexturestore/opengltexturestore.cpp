#include "opengltexturestore.h"
#include <QImage>

OpenGLTextureStore::OpenGLTextureStore()
    : OpenGLTextureStore::BasePathManagingObjectStore()
{
    createDefaultTexture();
}

OpenGLTextureStore::~OpenGLTextureStore()
{
    // Clearing the hash will destroy all the textures.
    m_ObjectHash.clear();
}

QWeakPointer<RenderSystem::NamedOpenGLTexture> OpenGLTextureStore::texture(const RenderSystem::TextureDefs::TextureId textureId) const
{
    return object(textureId).toWeakRef();
}

QWeakPointer<RenderSystem::NamedOpenGLTexture> OpenGLTextureStore::texture(const QString &path) const
{
    return object(textureIdFromPath(path)).toWeakRef();
}

OpenGLTextureStore::TextureId OpenGLTextureStore::addTexture(const QImage& image, const QString& path)
{
    if ( path.isEmpty() || image.isNull() )
    {
        return INVALID_ID;
    }

    TextureId existingTexture = objectIdFromPath(path);
    if ( existingTexture != INVALID_ID )
    {
        destroy(existingTexture);
    }

    return createWithPath(path, image);
}

OpenGLTextureStore::TextureId OpenGLTextureStore::createBlankTexture(const QString &path)
{
    if ( path.isEmpty() )
    {
        return INVALID_ID;
    }

    TextureId existingTexture = objectIdFromPath(path);
    if ( existingTexture != INVALID_ID )
    {
        destroy(existingTexture);
    }

    return createWithPath(path, QOpenGLTexture::Target2D);
}

void OpenGLTextureStore::removeTexture(const TextureId id)
{
    if ( id == INVALID_ID )
    {
        return;
    }

    destroy(id);
}

bool OpenGLTextureStore::containsTexture(const TextureId id) const
{
    return BaseObjectStore::contains(id);
}

bool OpenGLTextureStore::containsTexture(const QString &path) const
{
    return contains(path);
}

OpenGLTextureStore::TextureId OpenGLTextureStore::textureIdFromPath(const QString& path) const
{
    return objectIdFromPath(path);
}

QString OpenGLTextureStore::texturePathFromId(const TextureId id) const
{
    return objectPathFromId(id);
}

void OpenGLTextureStore::objectCreated(const ObjectId id)
{
    storePathForCreatedObject(id);
}

void OpenGLTextureStore::objectAboutToBeDestroyed(const ObjectId id)
{
    removePathForDestroyedObject(id);
}

void OpenGLTextureStore::createDefaultTexture()
{
    QImage image(":/textures/_internal/error.png");

    if ( image.isNull() )
    {
        qFatal("Internal error texture could not be loaded!");
        return;
    }

    createDefaultObject(QString(), image);
}
