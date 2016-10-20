#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QOpenGLWindow>
#include "stores/shaderstore.h"
#include "stores/texturestore.h"
#include "scene/scene.h"
#include "camera/scenecamera.h"
#include "simplerenderpassclassifier.h"
#include "scenerenderer/scenerenderer.h"
#include "camera/cameracontroller.h"
#include "controller-adapters/keymap.h"
#include "controller-adapters/mouseeventmap.h"

class MainWindow : public QOpenGLWindow
{
    Q_OBJECT

public:
    explicit MainWindow();
    ~MainWindow();

protected:
    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void resizeGL(int w, int h) override;
    virtual void mousePressEvent(QMouseEvent *e) override;
    virtual void mouseReleaseEvent(QMouseEvent *e) override;

private:
    void destroy();
    void initShaders();
    void initTextures();
    void initLocalOpenGlSettings();
    void initRenderer();
    void initScene();
    void initSceneRenderer();
    void initCameraController();
    void initKeyMap();
    void initMouseEventMap();

    NS_MODEL::ShaderStore* m_pShaderStore;
    NS_MODEL::TextureStore* m_pTextureStore;
    quint16 m_iDefaultShader;
    quint32 m_iDefaultTexture;

    NS_MODEL::Scene* m_pScene;
    NS_MODEL::SceneCamera* m_pCamera;
    SimpleRenderPassClassifier m_RenderPassClassifier;
    NS_MODEL::SceneRenderer* m_pSceneRenderer;

    NS_MODEL::CameraController* m_pCameraController;
    NS_MODEL::KeyMap* m_pKeyMap;
    NS_MODEL::MouseEventMap* m_pMouseEventMap;
};

#endif // MAINWINDOW_H
