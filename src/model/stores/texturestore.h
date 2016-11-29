#ifndef TEXTURESTORE_H
#define TEXTURESTORE_H

#include "model_global.h"
#include <QHash>
#include "opengl/opengltexture.h"
#include "functors/itextureretrievalfunctor.h"

namespace Model
{
    class MODELSHARED_EXPORT TextureStore : public Renderer::ITextureRetrievalFunctor
    {
    public:
        TextureStore();
        ~TextureStore();

        virtual Renderer::OpenGLTexturePointer operator ()(quint32 textureId) const override;
        Renderer::OpenGLTexturePointer getTexture(quint32 textureId) const;
        Renderer::OpenGLTexturePointer createTextureFromFile(const QString &path);
        Renderer::OpenGLTexturePointer createEmptyTexture(const QString& path);
        quint32 getTextureId(const QString &path) const;
        Renderer::OpenGLTexturePointer getTexture(const QString &path) const;

    private:
        quint32 acquireNextTextureId();
        void processCreatedTexture(const Renderer::OpenGLTexturePointer& texture, const QString& path);

        quint32 m_iNextTextureId;
        QHash<quint32, Renderer::OpenGLTexturePointer> m_TextureTable;
        QHash<QString, quint32> m_TexturePathTable;
    };
}

#endif // TEXTURESTORE_H
