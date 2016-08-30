#ifndef DEMOGLWINDOW_H
#define DEMOGLWINDOW_H

#include "renderer_global.h"
#include <QOpenGLWindow>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

namespace NS_RENDERER
{
    class RenderModelBatch;
    class TempSpec;
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

        QOpenGLShaderProgram *m_program;

        RenderModelBatch* m_pBatch;
        TempSpec*       m_pTempSpec;
    };
}

#endif // DEMOGLWINDOW_H
