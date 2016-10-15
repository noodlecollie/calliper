#include "opengltexture.h"

namespace NS_RENDERER
{
    OpenGLTexture::OpenGLTexture(quint32 id, QOpenGLTexture::Target target) :
        QOpenGLTexture(target), m_iId(id)
    {
        Q_ASSERT_X(m_iId > 0, Q_FUNC_INFO, "Texture ID should not be zero!");
    }

    OpenGLTexture::OpenGLTexture(quint32 id, const QImage &image, QOpenGLTexture::MipMapGeneration genMipMaps) :
        QOpenGLTexture(image, genMipMaps), m_iId(id)
    {
        Q_ASSERT_X(m_iId > 0, Q_FUNC_INFO, "Texture ID should not be zero!");
    }

    quint32 OpenGLTexture::textureStoreId() const
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

    QSize OpenGLTexture::size() const
    {
        return QSize(width(), height());
    }
}
