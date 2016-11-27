#ifndef ITEXTURERETRIEVALFUNCTOR_H
#define ITEXTURERETRIEVALFUNCTOR_H

#include "renderer_global.h"
#include <QSharedPointer>
#include "opengl/opengltexture.h"

namespace Renderer
{
    typedef QSharedPointer<OpenGLTexture> OpenGLTexturePointer;

    class ITextureRetrievalFunctor
    {
    public:
        virtual ~ITextureRetrievalFunctor() {}

        // Get a pointer to a texture by ID.
        // If the ID is 0, return an error texture.
        virtual OpenGLTexturePointer operator ()(quint32 textureId) const = 0;
    };
}

#endif // ITEXTURERETRIEVALFUNCTOR_H
