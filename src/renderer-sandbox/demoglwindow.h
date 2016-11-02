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
#include "stores/shaderstore.h"
#include "stores/texturestore.h"
#include "scenerenderer/scenerenderer.h"
#include "camera/scenecamera.h"
#include "controller-adapters/keymap.h"
#include "camera/cameracontroller.h"
#include "controller-adapters/mouseeventmap.h"
#include "genericbrush/genericbrush.h"

class TempSpec;

namespace Renderer
{
    class OpenGLShaderProgram;
}

namespace Model
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

    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseReleaseEvent(QMouseEvent *e);

private slots:
    void timeout();

private:
    void buildCube();

    TempSpec*   m_pTempSpec;
    QOpenGLTexture* m_pTexture;
    QTimer m_Timer;
    QTime m_FrameTime;
    int m_iTris;
    quint32 m_iCounter;
    QTimer m_HidingTimer;
    Model::ShaderStore* m_pShaderStore;
    Model::TextureStore* m_pTextureStore;

    Model::DebugCube*    m_pSceneObject;
    Model::Scene*        m_pScene;
    Model::SceneRenderer* m_pSceneRenderer;
    Model::SceneCamera* m_pCamera;
    Model::KeyMap* m_pKeyMap;
    Model::CameraController* m_pCameraController;
    Model::MouseEventMap* m_pMouseMap;

    Model::GenericBrush* m_pBrush;

    QPoint m_LastMousePos;
    bool m_bMouseGrab;
};

#endif // DEMOGLWINDOW_H
