#ifndef NAMEDOPENGLTEXTURE_H
#define NAMEDOPENGLTEXTURE_H

#include "rendersystem_global.h"

#include <QOpenGLTexture>

namespace RenderSystem
{
    class RENDERSYSTEMSHARED_EXPORT NamedOpenGLTexture : public QOpenGLTexture
    {
    public:
        NamedOpenGLTexture(const QString& path, QOpenGLTexture::Target target);
        NamedOpenGLTexture(const QString& path, const QImage &image, QOpenGLTexture::MipMapGeneration genMipMaps = GenerateMipMaps);

        QString path() const;
        QSize size() const;

    private:
        NamedOpenGLTexture(const NamedOpenGLTexture& other) = delete;
        NamedOpenGLTexture& operator =(const NamedOpenGLTexture& other) = delete;

        const QString m_strPath;
    };
}

#endif // NAMEDOPENGLTEXTURE_H
