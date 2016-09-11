#ifndef ISHADERRETRIEVALFUNCTOR_H
#define ISHADERRETRIEVALFUNCTOR_H

#include "renderer_global.h"
#include "opengl/openglshaderprogram.h"

namespace NS_RENDERER
{
    class IShaderRetrievalFunctor
    {
    public:
        virtual ~IShaderRetrievalFunctor() {}

        // Get a pointer to a shader by ID.
        virtual OpenGLShaderProgram* operator ()(quint16 shaderId) const = 0;
    };
}

#endif // ISHADERRETRIEVALFUNCTOR_H