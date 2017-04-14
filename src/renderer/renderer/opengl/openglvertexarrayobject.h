#ifndef OPENGLVERTEXARRAYOBJECT_H
#define OPENGLVERTEXARRAYOBJECT_H

#include "renderer_global.h"
#include <QOpenGLFunctions>

namespace Renderer
{
    class RENDERERSHARED_EXPORT OpenGLVertexArrayObject
    {
    public:
        OpenGLVertexArrayObject();
        ~OpenGLVertexArrayObject();

        bool create();
        void destroy();
        bool isCreated() const;

        void bind();
        void release();

    private:
        GLuint  m_iVAOID;
    };
}

#endif // OPENGLVERTEXARRAYOBJECT_H
