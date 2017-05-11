#include "opengltexturestore.h"

OpenGLTextureStore::TextureId OpenGLTextureStore::addTexture(const QImage& image, const QString& path)
{
    TextureId existingTexture = m_PathToTextureIdMap.value(path, INVALID_ID);
    if ( existingTexture != INVALID_ID )
    {
        destroy(existingTexture);
    }

    return create(path, image);
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
    return contains(id);
}

OpenGLTextureStore::TextureId OpenGLTextureStore::textureIdFromPath(const QString& path) const
{
    return m_PathToTextureIdMap.value(path, INVALID_ID);
}

QString OpenGLTextureStore::texturePathFromId(const TextureId id) const
{
    QSharedPointer<NamedOpenGLTexture> texture = object(id);
    Q_ASSERT_X(texture, Q_FUNC_INFO, "Created texture was invalid!");
    if ( !texture )
    {
        return QString();
    }

    return texture->path();
}

void OpenGLTextureStore::objectCreated(const ObjectId id)
{
    QSharedPointer<NamedOpenGLTexture> texture = object(id);
    Q_ASSERT_X(texture, Q_FUNC_INFO, "Created texture was invalid!");
    if ( !texture )
    {
        return;
    }

    Q_ASSERT_X(!m_PathToTextureIdMap.contains(texture->path()), Q_FUNC_INFO, "Path for new texture already exists!");

    m_PathToTextureIdMap.insert(texture->path(), id);
}

void OpenGLTextureStore::objectAboutToBeDestroyed(const ObjectId id)
{
    QSharedPointer<NamedOpenGLTexture> texture = object(id);
    Q_ASSERT_X(texture, Q_FUNC_INFO, "Created texture was invalid!");
    if ( !texture )
    {
        return;
    }

    m_PathToTextureIdMap.remove(texture->path());
}
