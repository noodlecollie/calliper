#include "storedopengltexture.h"

namespace RenderSystem
{
    StoredOpenGLTexture::StoredOpenGLTexture(const ObjectId id, const QImage &image, QOpenGLTexture::MipMapGeneration genMipMaps)
        : QOpenGLTexture(image, genMipMaps),
          m_nId(id)
    {
    }

    StoredOpenGLTexture::StoredOpenGLTexture(const ObjectId id, QOpenGLTexture::Target target)
        : QOpenGLTexture(target),
          m_nId(id)
    {
    }

    StoredOpenGLTexture::ObjectId StoredOpenGLTexture::storeId() const
    {
        return m_nId;
    }
}
