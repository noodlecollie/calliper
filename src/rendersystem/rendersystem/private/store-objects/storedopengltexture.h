#ifndef STOREDOPENGLTEXTURE_H
#define STOREDOPENGLTEXTURE_H

#include <QOpenGLTexture>

class StoredOpenGLTexture : public QOpenGLTexture
{
public:
    typedef quint32 ObjectId;

    StoredOpenGLTexture(const ObjectId id, const QString& path, QOpenGLTexture::Target target);
    StoredOpenGLTexture(const ObjectId id, const QString& path, const QImage &image, QOpenGLTexture::MipMapGeneration genMipMaps = GenerateMipMaps);

    ObjectId storeId() const;
    QString path() const;

private:
    StoredOpenGLTexture(const StoredOpenGLTexture& other) = delete;
    StoredOpenGLTexture& operator =(const StoredOpenGLTexture& other) = delete;

    const ObjectId m_nId;
    const QString m_strPath;
};

#endif // STOREDOPENGLTEXTURE_H
