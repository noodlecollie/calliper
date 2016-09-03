#include "opengltexture.h"

namespace NS_RENDERER
{
    OpenGLTexture::OpenGLTexture(QOpenGLTexture::Target target) :
        QOpenGLTexture(target), m_iId(0)
    {

    }

    OpenGLTexture::OpenGLTexture(const QImage &image, QOpenGLTexture::MipMapGeneration genMipMaps) :
        QOpenGLTexture(image, genMipMaps), m_iId(0)
    {

    }

    quint64 OpenGLTexture::textureStoreId() const
    {
        return m_iId;
    }

    QString OpenGLTexture::path() const
    {
        return m_szPath;
    }

    void OpenGLTexture::setPath(const QString &path)
    {
        m_szPath = path;
    }
}
