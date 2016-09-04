#ifndef OPENGLTEXTURE_H
#define OPENGLTEXTURE_H

#include "renderer_global.h"
#include <QOpenGLTexture>

namespace NS_RENDERER
{
    class RENDERERSHARED_EXPORT OpenGLTexture : public QOpenGLTexture
    {
    public:
        OpenGLTexture(QOpenGLTexture::Target target);
        OpenGLTexture(const QImage &image, QOpenGLTexture::MipMapGeneration genMipMaps = GenerateMipMaps);

        quint64 textureStoreId() const;

        QString path() const;
        void setPath(const QString &path);

    private:
        quint64 m_iId;
        QString m_szPath;
    };
}

#endif // OPENGLTEXTURE_H
