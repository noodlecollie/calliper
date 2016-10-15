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

    m_Timer.setInterval((int)(10.0f/6.0f));
    m_Timer.setSingleShot(false);
    connect(&m_Timer, SIGNAL(timeout()), this, SLOT(update()));

    m_HidingTimer.setInterval(50);
    m_HidingTimer.setSingleShot(false);
    connect(&m_HidingTimer, &QTimer::timeout, this, &DemoGLWindow::timeout);
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

    GLTRY(f->glEnable(GL_DEPTH_TEST));
    GLTRY(f->glFrontFace(GL_CCW));
    GLTRY(f->glCullFace(GL_BACK));

    m_pShaderStore = new ShaderStore();
    m_pTextureStore = new TextureStore();

    m_pShaderStore->addShaderProgram<TempShader>();
    m_pShaderStore->addShaderProgram<ColorShader>();

    Global::initialise();
    IRenderer* renderer = Global::renderer();
    renderer->setShaderFunctor(m_pShaderStore);
    renderer->setTextureFunctor(m_pTextureStore);

    m_pScene = new Scene(m_pShaderStore, m_pTextureStore, this);
    m_pSceneObject = m_pScene->createSceneObject<DebugCube>(m_pScene->rootObject());
    m_pSceneObject->setRadius(0.2f);
    m_pSceneObject->hierarchy().setPosition(QVector3D(0.3f, 0, 0));
    m_pSceneObject->setDrawFrame(true);
    m_pSceneObject->setObjectName("Cube");

    buildCube();

    m_pCamera = m_pScene->createSceneObject<SceneCamera>(m_pScene->rootObject());
    CameraLens lens(CameraLens::Orthographic);
    lens.setTopPlane(1.0f);
    lens.setBottomPlane(-1.0f);
    lens.setLeftPlane(-1.0f);
    lens.setRightPlane(1.0f);
    lens.setNearPlane(0.01f);
    lens.setFarPlane(1.0f);
    m_pCamera->setLens(lens);

    m_pSceneRenderer = new SceneRenderer(m_pShaderStore, m_pTextureStore, &passClassifier, renderer, m_pScene);
    m_pSceneRenderer->setDefaultShaderId(1);
    m_pSceneRenderer->setDefaultTextureId(1);

    m_FrameTime = QTime::currentTime();
    m_Timer.start();
    m_HidingTimer.start();
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
    qDebug() << "FPS:" << (1000.0f/(float)(newMs - oldMs)) << "with" << (m_iTris * m_iTris) << "triangles";
    m_FrameTime = curTime;

    GL_CURRENT_F;

    const qreal retinaScale = devicePixelRatio();
    GLTRY(f->glViewport(0, 0, width() * retinaScale, height() * retinaScale));

    GLTRY(f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    m_pSceneRenderer->render(QMatrix4x4(), QMatrix4x4());
}

void DemoGLWindow::timeout()
{
    makeCurrent();
    buildCube();
    doneCurrent();
}

void DemoGLWindow::buildCube()
{
    static float rot = 0.0f;

    m_pSceneObject->hierarchy().setRotation(EulerAngle(rot,rot,rot));
    rot += 5.0f;
    if ( rot >= 360.0f )
        rot -= 360.0f;
}
