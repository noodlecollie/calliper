#ifndef DEMOGLWINDOW_H
#define DEMOGLWINDOW_H

#include "renderer_global.h"
#include <QOpenGLWindow>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include "opengluniformbuffer.h"

namespace NS_RENDERER
{
    class RenderModelBatch;
}

class TempSpec;

class DemoGLWindow : public QOpenGLWindow
{
public:
    DemoGLWindow();
    virtual ~DemoGLWindow();

protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

private:
    QOpenGLShaderProgram *m_program;
    GLuint  m_iVAOID;

    NS_RENDERER::RenderModelBatch* m_pBatch;
    TempSpec*   m_pTempSpec;
};

#endif // DEMOGLWINDOW_H
