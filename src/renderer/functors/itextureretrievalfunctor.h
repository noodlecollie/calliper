#ifndef ITEXTURERETRIEVALFUNCTOR_H
#define ITEXTURERETRIEVALFUNCTOR_H

#include "renderer_global.h"
#include <QSharedPointer>
#include "opengl/opengltexture.h"

namespace NS_RENDERER
{
    typedef QSharedPointer<OpenGLTexture> OpenGLTexturePointer;

    class ITextureRetrievalFunctor
    {
    public:
        virtual ~ITextureRetrievalFunctor() {}

        // Get a pointer to a texture by ID.
        virtual OpenGLTexturePointer operator ()(quint64 textureId) const = 0;
    };
}

#endif // ITEXTURERETRIEVALFUNCTOR_H