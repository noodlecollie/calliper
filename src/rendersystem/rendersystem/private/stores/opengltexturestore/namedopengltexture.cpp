#include "namedopengltexture.h"

NamedOpenGLTexture::NamedOpenGLTexture(const QString& path, const QImage &image, QOpenGLTexture::MipMapGeneration genMipMaps)
    : QOpenGLTexture(image, genMipMaps),
      m_strPath(path)
{
}

NamedOpenGLTexture::NamedOpenGLTexture(const QString& path, QOpenGLTexture::Target target)
    : QOpenGLTexture(target),
      m_strPath(path)
{
}

QString NamedOpenGLTexture::path() const
{
    return m_strPath;
}
