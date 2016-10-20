#include "demoglwindow.h"
#include <QScreen>
#include <QOpenGLFunctions_4_1_Core>
#include "opengl/openglerrors.h"
#include <QtDebug>
#include "shaders/ishaderspec.h"
#include "shaders/shaderdefs.h"
#include "opengl/openglhelpers.h"
#include "shaders/debugscreenspaceshader.h"
#include "tempshader.h"
#include "geometry/geometrybuilder.h"
#include "irenderer.h"
#include "rendermodel/rendererinputobjectparams.h"
#include "scene/scene.h"
#include "sceneobjects/debugcube.h"
#include "colorshader.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include "sceneobjects/debugtriangle.h"

using namespace NS_RENDERER;
using namespace NS_MODEL;

class Classifier : public IRenderPassClassifier
{
public:
    virtual int classify(quint32) const
    {
        return 0;
    }
};

Classifier passClassifier;

DemoGLWindow::DemoGLWindow()
{
    m_pTempSpec = nullptr;
    m_pTexture = nullptr;
    m_iCounter = 1;
    m_iTris = 1;
    m_bMouseGrab = false;

    m_pCamera = nullptr;
    m_pCameraController = nullptr;
    m_pKeyMap = nullptr;
    m_pScene = nullptr;
    m_pSceneObject = nullptr;
    m_pSceneRenderer = nullptr;
    m_pShaderStore = nullptr;
    m_pTempSpec = nullptr;
    m_pTexture = nullptr;
    m_pTextureStore = nullptr;

    m_Timer.setInterval((int)(10.0f/6.0f));
    m_Timer.setSingleShot(false);
    connect(&m_Timer, SIGNAL(timeout()), this, SLOT(update()));
}

DemoGLWindow::~DemoGLWindow()
{
    makeCurrent();
    Q_ASSERT_X(QOpenGLContext::currentContext(), Q_FUNC_INFO, "Need a current context to clean things up!");

    Global::shutdown();

    delete m_pSceneRenderer;
    delete m_pScene;
    delete m_pShaderStore;
    delete m_pTextureStore;

    doneCurrent();
}

void DemoGLWindow::initializeGL()
{
    qDebug() << OpenGLErrors::debugOpenGLCapabilities().toLatin1().constData();
    GL_CURRENT_F;

    GLTRY(f->glEnable(GL_CULL_FACE));
    GLTRY(f->glCullFace(GL_BACK));

    GLTRY(f->glEnable(GL_DEPTH_TEST));
    GLTRY(f->glDepthFunc(GL_LESS));

    m_pShaderStore = new ShaderStore();
    m_pTextureStore = new TextureStore();

    m_pShaderStore->addShaderProgram<TempShader>();
    m_pShaderStore->addShaderProgram<ColorShader>();

    OpenGLTexturePointer tex = m_pTextureStore->createTexture(":/obsolete-opaque.png");
    qDebug() << "Texture" << tex->path() << "has ID" << tex->textureStoreId();

    Global::initialise();
    IRenderer* renderer = Global::renderer();
    renderer->setShaderFunctor(m_pShaderStore);
    renderer->setTextureFunctor(m_pTextureStore);

    m_pScene = new Scene(m_pShaderStore, m_pTextureStore, this);
    m_pSceneObject = m_pScene->createSceneObject<DebugCube>(m_pScene->rootObject());
    m_pSceneObject->setRadius(32.0f);
    m_pSceneObject->hierarchy().setPosition(QVector3D(0, 0, 0));
    m_pSceneObject->setDrawFrame(true);
    m_pSceneObject->setObjectName("Cube");

    m_pCamera = m_pScene->createSceneObject<SceneCamera>(m_pScene->rootObject());
    m_pCamera->hierarchy().setPosition(QVector3D(-40,0,0));
//    CameraLens lens(CameraLens::Orthographic);
//    lens.setTopPlane(1.0f);
//    lens.setBottomPlane(-1.0f);
//    lens.setLeftPlane(-1.0f);
//    lens.setRightPlane(1.0f);
//    lens.setNearPlane(-1.0f);
//    lens.setFarPlane(1.0f);
//    m_pCamera->setLens(lens);

    m_pSceneRenderer = new SceneRenderer(m_pShaderStore, m_pTextureStore, &passClassifier, renderer, m_pScene);
    m_pSceneRenderer->setDefaultShaderId(1);
    m_pSceneRenderer->setDefaultTextureId(1);

    m_pCameraController = new CameraController(this);
    m_pCameraController->setCamera(m_pCamera);
    m_pCameraController->setStrafeSpeed(50.0f);
    m_pCameraController->setForwardSpeed(50.0f);
    m_pCameraController->setVerticalSpeed(50.0f);
    m_pCameraController->setEnabled(true);

    m_pKeyMap = new KeyMap(this);
    connect(m_pKeyMap->addKeyMap(Qt::Key_W), &KeySignalSender::keyEvent,
            m_pCameraController, &CameraController::moveForward);
    connect(m_pKeyMap->addKeyMap(Qt::Key_S), &KeySignalSender::keyEvent,
            m_pCameraController, &CameraController::moveBackward);
    connect(m_pKeyMap->addKeyMap(Qt::Key_A), &KeySignalSender::keyEvent,
            m_pCameraController, &CameraController::moveLeft);
    connect(m_pKeyMap->addKeyMap(Qt::Key_D), &KeySignalSender::keyEvent,
            m_pCameraController, &CameraController::moveRight);
    connect(m_pKeyMap->addKeyMap(Qt::Key_Q), &KeySignalSender::keyEvent,
            m_pCameraController, &CameraController::moveUp);
    connect(m_pKeyMap->addKeyMap(Qt::Key_Z), &KeySignalSender::keyEvent,
            m_pCameraController, &CameraController::moveDown);

    installEventFilter(m_pKeyMap);

    m_FrameTime = QTime::currentTime();
    m_Timer.start();
}

void DemoGLWindow::resizeGL(int w, int h)
{
    QOpenGLWindow::resizeGL(w,h);
}

void DemoGLWindow::paintGL()
{
    QTime curTime = QTime::currentTime();
    int oldMs = m_FrameTime.msecsSinceStartOfDay();
    int newMs = curTime.msecsSinceStartOfDay();
    //qDebug() << "FPS:" << (1000.0f/(float)(newMs - oldMs)) << "with" << (m_iTris * m_iTris) << "triangles";
    m_FrameTime = curTime;

    GL_CURRENT_F;

    const qreal retinaScale = devicePixelRatio();
    GLTRY(f->glViewport(0, 0, width() * retinaScale, height() * retinaScale));

    GLTRY(f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    m_pSceneRenderer->render(m_pCamera);
}

void DemoGLWindow::timeout()
{
    makeCurrent();
    buildCube();
    doneCurrent();
}

void DemoGLWindow::buildCube()
{
}

void DemoGLWindow::mouseMoveEvent(QMouseEvent *e)
{
    if ( !m_bMouseGrab )
    {
        QOpenGLWindow::mouseMoveEvent(e);
        return;
    }

    if ( !m_pCameraController )
        return;

    int x = e->pos().x() - m_LastMousePos.x();
    int y = e->pos().y() - m_LastMousePos.y();

    m_pCameraController->addPitch(y);
    m_pCameraController->addYaw(-x);

    m_LastMousePos = e->pos();
}

void DemoGLWindow::mousePressEvent(QMouseEvent *e)
{
    if ( e->button() == Qt::LeftButton )
    {
        m_bMouseGrab = true;
        m_LastMousePos = e->pos();
    }
    else
    {
        QOpenGLWindow::mousePressEvent(e);
    }
}

void DemoGLWindow::mouseReleaseEvent(QMouseEvent *e)
{
    if ( e->button() == Qt::LeftButton )
    {
        m_bMouseGrab = false;
    }
    else
    {
        QOpenGLWindow::mouseReleaseEvent(e);
    }
}
