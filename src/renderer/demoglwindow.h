#ifndef DEMOGLWINDOW_H
#define DEMOGLWINDOW_H

#include "renderer_global.h"
#include <QOpenGLWindow>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

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
    GLuint m_nrmAttr;
    GLuint m_colAttr;
    GLuint m_modelWorldUniform;
    GLuint m_worldCameraUniform;
    GLuint m_coordTransformUniform;
    GLuint m_projectionUniform;

    QOpenGLShaderProgram *m_program;

    QOpenGLBuffer*  m_pVertexBuffer;
    QOpenGLBuffer*  m_pIndexBuffer;
};

#endif // DEMOGLWINDOW_H
