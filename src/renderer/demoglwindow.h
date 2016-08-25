#ifndef DEMOGLWINDOW_H
#define DEMOGLWINDOW_H

#include "renderer_global.h"
#include <QOpenGLWindow>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

namespace NS_RENDERER
{
    class RENDERERSHARED_EXPORT DemoGLWindow : public QOpenGLWindow
    {
    public:
        DemoGLWindow();
        virtual ~DemoGLWindow();

    protected:
        virtual void initializeGL();
        virtual void resizeGL(int w, int h);
        virtual void paintGL();

    private:
        GLuint m_posAttr;
        GLuint m_colAttr;
        GLuint m_vao;

        QOpenGLShaderProgram *m_program;
        GLuint m_VertexShaderHandle;
        GLuint m_FragmentShaderHandle;
        GLuint m_ShaderHandle;

        QOpenGLBuffer*  m_pVertexBuffer;
        QOpenGLBuffer*  m_pIndexBuffer;
        QOpenGLBuffer*  m_pColourBuffer;
    };
}

#endif // DEMOGLWINDOW_H
