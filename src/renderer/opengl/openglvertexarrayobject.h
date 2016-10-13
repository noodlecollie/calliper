#ifndef OPENGLVERTEXARRAYOBJECT_H
#define OPENGLVERTEXARRAYOBJECT_H

#include "renderer_global.h"
#include <QOpenGLFunctions>

namespace NS_RENDERER
{
    class OpenGLVertexArrayObject
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
