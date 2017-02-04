#ifndef ISHADERRETRIEVALFUNCTOR_H
#define ISHADERRETRIEVALFUNCTOR_H

#include "renderer_global.h"
#include "renderer/opengl/openglshaderprogram.h"

namespace Renderer
{
    class IShaderRetrievalFunctor
    {
    public:
        virtual ~IShaderRetrievalFunctor() {}

        // Get a pointer to a shader by ID.
        // If the ID is 0, return the default shader.
        // This could be a fallback, or it could be the current shader
        // for this frame (eg. lit, wireframe, etc.).
        virtual OpenGLShaderProgram* operator ()(quint16 shaderId) const = 0;
    };
}

#endif // ISHADERRETRIEVALFUNCTOR_H
