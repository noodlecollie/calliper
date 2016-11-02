#ifndef TEXTURESTORE_H
#define TEXTURESTORE_H

#include "model_global.h"
#include <QHash>
#include "opengl/opengltexture.h"
#include "functors/itextureretrievalfunctor.h"

namespace NS_MODEL
{
    class MODELSHARED_EXPORT TextureStore : public Renderer::ITextureRetrievalFunctor
    {
    public:
        TextureStore();
        ~TextureStore();

        virtual Renderer::OpenGLTexturePointer operator ()(quint64 textureId) const override;
        Renderer::OpenGLTexturePointer getTexture(quint32 textureId) const;
        Renderer::OpenGLTexturePointer createTexture(const QString &path);
        quint32 getTextureId(const QString &path) const;
        Renderer::OpenGLTexturePointer getTexture(const QString &path) const;

        Renderer::OpenGLTexturePointer createDefaultTexture(const QString& path);

    private:
        quint32 acquireNextTextureId();
        Renderer::OpenGLTexturePointer createTextureInternal(const QString &path, quint32 id);

        quint32 m_iNextTextureId;
        QHash<quint32, Renderer::OpenGLTexturePointer> m_TextureTable;
        QHash<QString, quint32> m_TexturePathTable;
    };
}

#endif // TEXTURESTORE_H
