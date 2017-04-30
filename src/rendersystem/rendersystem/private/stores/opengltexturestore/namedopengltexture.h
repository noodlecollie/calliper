#ifndef NAMEDOPENGLTEXTURE_H
#define NAMEDOPENGLTEXTURE_H

#include <QOpenGLTexture>

class NamedOpenGLTexture : public QOpenGLTexture
{
public:
    NamedOpenGLTexture(const QString& path, QOpenGLTexture::Target target);
    NamedOpenGLTexture(const QString& path, const QImage &image, QOpenGLTexture::MipMapGeneration genMipMaps = GenerateMipMaps);

    QString path() const;

private:
    NamedOpenGLTexture(const NamedOpenGLTexture& other) = delete;
    NamedOpenGLTexture& operator =(const NamedOpenGLTexture& other) = delete;

    const QString m_strPath;
};

#endif // NAMEDOPENGLTEXTURE_H
