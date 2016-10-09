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
#include <QTimer>
#include <QTime>
#include "model_global.h"

class TempSpec;

namespace NS_RENDERER
{
    class OpenGLShaderProgram;
}

namespace NS_MODEL
{
    class Scene;
    class DebugCube;
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

private slots:
    void timeout();

private:
    void buildCube();
    GLuint  m_iVAOID;

    TempSpec*   m_pTempSpec;
    QOpenGLTexture* m_pTexture;
    QTimer m_Timer;
    QTime m_FrameTime;
    int m_iTris;
    quint32 m_iCounter;
    QTimer m_HidingTimer;

    NS_MODEL::DebugCube*    m_pSceneObject;
    NS_MODEL::Scene*        m_pScene;
};

#endif // DEMOGLWINDOW_H
