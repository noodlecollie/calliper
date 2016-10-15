#ifndef OPENGLTEXTURE_H
#define OPENGLTEXTURE_H

#include "renderer_global.h"
#include <QOpenGLTexture>

namespace NS_RENDERER
{
    class RENDERERSHARED_EXPORT OpenGLTexture : public QOpenGLTexture
    {
    public:
        OpenGLTexture(quint32 id, QOpenGLTexture::Target target);
        OpenGLTexture(quint32 id, const QImage &image, QOpenGLTexture::MipMapGeneration genMipMaps = GenerateMipMaps);

        quint32 textureStoreId() const;

        QString path() const;
        void setPath(const QString &path);

        QSize size() const;

    private:
        quint32 m_iId;
        QString m_szPath;
    };
}

#endif // OPENGLTEXTURE_H
