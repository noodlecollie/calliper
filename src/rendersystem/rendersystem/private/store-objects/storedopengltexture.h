#ifndef STOREDOPENGLTEXTURE_H
#define STOREDOPENGLTEXTURE_H

#include "rendersystem_global.h"

#include <QOpenGLTexture>

namespace RenderSystem
{
    class StoredOpenGLTexture : public QOpenGLTexture
    {
    public:
        typedef quint32 ObjectId;

        StoredOpenGLTexture(const ObjectId id, QOpenGLTexture::Target target);
        StoredOpenGLTexture(const ObjectId id, const QImage &image, QOpenGLTexture::MipMapGeneration genMipMaps = GenerateMipMaps);

        ObjectId storeId() const;

    private:
        StoredOpenGLTexture(const StoredOpenGLTexture& other) = delete;
        StoredOpenGLTexture& operator =(const StoredOpenGLTexture& other) = delete;

        const ObjectId m_nId;
    };
}

#endif // STOREDOPENGLTEXTURE_H
