#include "storedopengltexture.h"

namespace RenderSystem
{
    StoredOpenGLTexture::StoredOpenGLTexture(const ObjectId id, const QString& path,
                                             const QImage &image, QOpenGLTexture::MipMapGeneration genMipMaps)
        : QOpenGLTexture(image, genMipMaps),
          m_nId(id),
          m_strPath(path)
    {
    }

    StoredOpenGLTexture::StoredOpenGLTexture(const ObjectId id, const QString& path, QOpenGLTexture::Target target)
        : QOpenGLTexture(target),
          m_nId(id),
          m_strPath(path)
    {
    }

    StoredOpenGLTexture::ObjectId StoredOpenGLTexture::storeId() const
    {
        return m_nId;
    }

    QString StoredOpenGLTexture::path() const
    {
        return m_strPath;
    }
}
