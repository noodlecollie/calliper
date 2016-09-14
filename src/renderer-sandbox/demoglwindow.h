#ifndef DEMOGLWINDOW_H
#define DEMOGLWINDOW_H

#include "renderer_global.h"
#include <QOpenGLWindow>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include "opengl/opengluniformbuffer.h"
#include <QImage>
#include <QOpenGLTexture>
#include "rendermodel/1-passlevel/rendermodelpass.h"

class TempSpec;

namespace NS_RENDERER
{
    class OpenGLShaderProgram;
}

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
    GLuint  m_iVAOID;

    NS_RENDERER::RenderModelPass*   m_pRenderModel;
    TempSpec*   m_pTempSpec;
    QOpenGLTexture* m_pTexture;
};

#endif // DEMOGLWINDOW_H
