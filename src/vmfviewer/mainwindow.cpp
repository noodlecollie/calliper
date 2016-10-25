#include "mainwindow.h"
#include "shaders/unlitshader.h"
#include "shaders/unlitpervertexcolorshader.h"
#include "opengl/openglerrors.h"
#include "opengl/openglhelpers.h"
#include "irenderer.h"
#include <QMouseEvent>
#include <QFile>
#include "keyvalues/keyvaluesparser.h"
#include "vmf/vmf.h"
#include <QtDebug>
#include "shaders/errorshader.h"

using namespace NS_MODEL;
using namespace NS_RENDERER;

MainWindow::MainWindow(const QString& filename) :
    QOpenGLWindow(),
    m_strFilename(filename),
    m_pShaderStore(nullptr),
    m_pTextureStore(nullptr),
    m_iDefaultTexture(0),
    m_pScene(nullptr),
    m_pCamera(nullptr),
    m_pSceneRenderer(nullptr),
    m_pCameraController(nullptr),
    m_pKeyMap(nullptr),
    m_pMouseEventMap(nullptr)
{
    resize(640, 480);
}

MainWindow::~MainWindow()
{
    makeCurrent();
    destroy();
    doneCurrent();
}

void MainWindow::destroy()
{
    delete m_pMouseEventMap;
    m_pMouseEventMap = nullptr;

    delete m_pKeyMap;
    m_pKeyMap = nullptr;

    delete m_pCameraController;
    m_pCameraController = nullptr;

    delete m_pSceneRenderer;
    m_pSceneRenderer = nullptr;

    m_pCamera = nullptr;
    delete m_pScene;
    m_pScene = nullptr;

    Global::shutdown();

    delete m_pShaderStore;
    m_pShaderStore = nullptr;

    delete m_pTextureStore;
    m_pTextureStore = nullptr;
}

void MainWindow::initializeGL()
{
    initLocalOpenGlSettings();

    m_pShaderStore = new ShaderStore();
    initShaders();

    m_pTextureStore = new TextureStore();
    initTextures();

    initRenderer();

    m_pScene = new BasicScene(m_pShaderStore, m_pTextureStore, this);
    initScene();

    m_pSceneRenderer = new SceneRenderer(m_pShaderStore, m_pTextureStore, &m_RenderPassClassifier,
                                         Global::renderer(), m_pScene);
    initSceneRenderer();

    m_pCameraController = new CameraController(this);
    initCameraController();

    m_pKeyMap = new KeyMap(this);
    initKeyMap();

    m_pMouseEventMap = new MouseEventMap(this);
    initMouseEventMap();

    loadVMF();
}

void MainWindow::paintGL()
{
    GL_CURRENT_F;
    GLTRY(f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    m_pSceneRenderer->render(m_pCamera);
}

void MainWindow::resizeGL(int w, int h)
{
    if ( !m_pCamera )
        return;

    CameraLens lens = m_pCamera->lens();
    lens.setAspectRatio(static_cast<float>(w)/static_cast<float>(h));
    m_pCamera->setLens(lens);
}

void MainWindow::initShaders()
{
    quint16 errorShader = m_pShaderStore->addShaderProgram<ErrorShader>();
    quint16 defaultShader = m_pShaderStore->addShaderProgram<UnlitShader>();

    m_DefaultShaderPalette.addItem(ShaderPalette::DefaultShader, errorShader);
    m_DefaultShaderPalette.addItem(ShaderPalette::UnlitPerVertexColor, m_pShaderStore->addShaderProgram<UnlitPerVertexColorShader>());
}

void MainWindow::initTextures()
{
    OpenGLTexturePointer tex = m_pTextureStore->createDefaultTexture(":model/textures/_ERROR_");
    m_iDefaultTexture = tex->textureStoreId();
}

void MainWindow::initLocalOpenGlSettings()
{
    GL_CURRENT_F;

    GLTRY(f->glEnable(GL_CULL_FACE));
    GLTRY(f->glCullFace(GL_BACK));

    GLTRY(f->glEnable(GL_DEPTH_TEST));
    GLTRY(f->glDepthFunc(GL_LESS));
}

void MainWindow::initRenderer()
{
    Global::initialise();
    Global::renderer()->setShaderFunctor(m_pShaderStore);
    Global::renderer()->setTextureFunctor(m_pTextureStore);
}

void MainWindow::initScene()
{
    m_pCamera = m_pScene->defaultCamera();
}

void MainWindow::initSceneRenderer()
{
    m_pSceneRenderer->setDefaultTextureId(m_iDefaultTexture);
    m_pSceneRenderer->setShaderPalette(m_DefaultShaderPalette);
}

void MainWindow::initCameraController()
{
    m_pCameraController->setCamera(m_pCamera);
    m_pCameraController->setStrafeSpeed(50.0f);
    m_pCameraController->setForwardSpeed(50.0f);
    m_pCameraController->setVerticalSpeed(50.0f);
    m_pCameraController->setEnabled(true);

    connect(m_pCameraController, SIGNAL(tickFinished()), this, SLOT(update()));
}

void MainWindow::initKeyMap()
{
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
    connect(m_pKeyMap->addKeyMap(Qt::Key_Escape), &KeySignalSender::keyEvent,
            this, &MainWindow::close);

    installEventFilter(m_pKeyMap);
}

void MainWindow::initMouseEventMap()
{
    m_pMouseEventMap->setHorizontalSensitivity(-1);
    connect(m_pMouseEventMap, &MouseEventMap::mouseMovedX, m_pCameraController, &CameraController::addYaw);
    connect(m_pMouseEventMap, &MouseEventMap::mouseMovedY, m_pCameraController, &CameraController::addPitch);
    connect(m_pMouseEventMap, SIGNAL(mouseMovedX(float)), this, SLOT(update()));
    connect(m_pMouseEventMap, SIGNAL(mouseMovedY(float)), this, SLOT(update()));

    installEventFilter(m_pMouseEventMap);
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if ( e->button() == Qt::LeftButton )
    {
        m_pMouseEventMap->setEnabled(true);
    }
    else
    {
        QOpenGLWindow::mousePressEvent(e);
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    if ( e->button() == Qt::LeftButton )
    {
        m_pMouseEventMap->setEnabled(false);
    }
    else
    {
        QOpenGLWindow::mousePressEvent(e);
    }
}

void MainWindow::loadVMF()
{
    using namespace NS_SERIALISATION;

    if ( m_strFilename.isNull() || m_strFilename.isEmpty() )
    {
        qDebug() << "No VMF file provided.";
        return;
    }

    QFile file(m_strFilename);
    if ( !file.open(QIODevice::ReadOnly) )
    {
        qDebug() << "Could not open VMF file" << m_strFilename;
        return;
    }

    QByteArray fileData = file.readAll();
    file.close();

    QJsonDocument vmfDoc;

    {
        KeyValuesParser kvParser(fileData);
        vmfDoc = kvParser.toJsonDocument();
    }

    VMF::createBrushes(vmfDoc, m_pScene->rootObject());
    qDebug() << "VMF" << m_strFilename << "loaded";
}
