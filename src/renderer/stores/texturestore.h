#ifndef TEXTURESTORE_H
#define TEXTURESTORE_H

#include "renderer_global.h"
#include <QHash>
#include "opengl/opengltexture.h"
#include <QSharedPointer>

namespace NS_RENDERER
{
    typedef QSharedPointer<OpenGLTexture> OpenGLTexturePointer;

    class RENDERERSHARED_EXPORT TextureStore
    {
    public:
        static TextureStore* getTextureStore();

        TextureStore();
        ~TextureStore();

        quint64 createTexture(const QImage &image, const QString &path);

        OpenGLTexturePointer texture(quint64 id) const;
        OpenGLTexturePointer texture(const QString &path) const;
        quint64 textureId(const QString &path) const;

    private:
        typedef QHash<quint64, OpenGLTexturePointer> TextureTable;
        typedef QHash<QString, quint64> TextureNameTable;

        void addBuiltInTextures();
        void addBuiltInTexture(const QString &path);
        void createErrorTexture();

        quint64 nextTextureId();

        TextureTable        m_TextureTable;
        TextureNameTable    m_TextureNameTable;

        quint64                 m_iNextTextureId;
        OpenGLTexturePointer    m_pErrorTexture;
    };
}

#endif // TEXTURESTORE_H
