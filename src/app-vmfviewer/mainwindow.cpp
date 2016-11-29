#include "mainwindow.h"
#include "shaders/unlitshader.h"
#include "shaders/unlitpervertexcolorshader.h"
#include "opengl/openglerrors.h"
#include "opengl/openglhelpers.h"
#include "irenderer.h"
#include <QMouseEvent>
#include <QFile>
#include "keyvalues/keyvaluesparser.h"
#include "vmf/vmfloader.h"
#include <QtDebug>
#include "shaders/errorshader.h"
#include "genericbrush/genericbrush.h"
#include "shaders/simplelitshader.h"
#include <QMessageBox>
#include <QtGlobal>
#include <QMap>
#include "vpk/vpkindextreerecord.h"
#include "vtf/vtfloader.h"

using namespace Model;
using namespace Renderer;

MainWindow::MainWindow(const QString& filename) :
    QOpenGLWindow(),
    m_strFilename(filename),
    m_pShaderStore(nullptr),
    m_pTextureStore(nullptr),
    m_pMaterialStore(nullptr),
    m_iPlaceholderMaterial(0),
    m_iErrorTextureId(0),
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

    delete m_pMaterialStore;
    m_pMaterialStore = nullptr;
}

void MainWindow::initializeGL()
{
    initLocalOpenGlSettings();

    m_pShaderStore = new ShaderStore();
    initShaders();

    m_pTextureStore = new TextureStore();
    initTextures();

    m_pMaterialStore = new MaterialStore();
    initMaterials();

    initRenderer();

    m_pScene = new BasicScene(m_pShaderStore, m_pTextureStore, m_pMaterialStore, this);
    initScene();

    m_pSceneRenderer = new SceneRenderer(m_pShaderStore, m_pTextureStore, m_pMaterialStore, &m_RenderPassClassifier,
                                         Global::renderer(), m_pScene);
    initSceneRenderer();

    m_pCameraController = new CameraController(this);
    initCameraController();

    m_pKeyMap = new KeyMap(this);
    initKeyMap();

    m_pMouseEventMap = new MouseEventMap(this);
    initMouseEventMap();

    importTextures();
    loadVMF();
    //processBrushes();
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
    m_DefaultShaderPalette.addItem(ShaderPalette::DefaultShader, m_pShaderStore->addShaderProgram<SimpleLitShader>());
    m_DefaultShaderPalette.addItem(ShaderPalette::UnlitPerVertexColor, m_pShaderStore->addShaderProgram<UnlitPerVertexColorShader>());
}

void MainWindow::initTextures()
{
    OpenGLTexturePointer defaultTexture = m_pTextureStore->createTextureFromFile(":model/textures/_ERROR_");
    Q_ASSERT_X(!defaultTexture.isNull(), Q_FUNC_INFO, "Unable to create default texture!");
    m_iErrorTextureId = defaultTexture->textureStoreId();

    m_pTextureStore->createTextureFromFile(":model/textures/dev/devwhite");
}

void MainWindow::initMaterials()
{
    // For now the string must be identical.
    Renderer::RenderMaterialPointer material = m_pMaterialStore->createMaterial(":model/textures/dev/devwhite");
    m_iPlaceholderMaterial = material->materialStoreId();

    material->addTexture(ShaderDefs::MainTexture, m_pTextureStore->getTextureId(":model/textures/dev/devwhite"));
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
    Global::renderer()->setMaterialFunctor(m_pMaterialStore);
}

void MainWindow::initScene()
{
    m_pCamera = m_pScene->defaultCamera();
}

void MainWindow::initSceneRenderer()
{
    m_pSceneRenderer->setDefaultTextureId(m_iErrorTextureId);
    m_pSceneRenderer->setShaderPalette(m_DefaultShaderPalette);
}

void MainWindow::initCameraController()
{
    m_pCameraController->setCamera(m_pCamera);
    m_pCameraController->setStrafeSpeed(100.0f);
    m_pCameraController->setForwardSpeed(100.0f);
    m_pCameraController->setVerticalSpeed(100.0f);
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
    using namespace FileFormats;
    using namespace ModelLoaders;

    if ( m_strFilename.isNull() || m_strFilename.isEmpty() )
    {
        qDebug() << "No VMF file provided.";
        QMessageBox::critical(nullptr, "Error", "No VMF file provided.");
        return;
    }

    QFile file(m_strFilename);
    if ( !file.open(QIODevice::ReadOnly) )
    {
        qDebug() << "Could not open VMF file" << m_strFilename;
        QMessageBox::critical(nullptr, "Error", QString("Could not open VMF file ") + m_strFilename);
        return;
    }

    QByteArray fileData = file.readAll();
    file.close();

    QJsonDocument vmfDoc;

    {
        KeyValuesParser kvParser(fileData);
        QString err;
        vmfDoc = kvParser.toJsonDocument(&err);

        if ( !err.isEmpty() )
        {
            qDebug().nospace() << "Error loading " << m_strFilename << ": " << err;
            return;
        }
    }

    VMFLoader::createBrushes(vmfDoc, m_pScene->rootObject());
    qDebug() << "VMF" << m_strFilename << "loaded";
}

void MainWindow::processBrushes()
{
    QList<GenericBrush*> brushes = m_pScene->rootObject()->findChildren<GenericBrush*>();

    foreach ( GenericBrush* brush, brushes )
    {
        float rand = (float)qrand()/(float)RAND_MAX;
        rand = 0.5f + (0.5f * rand);
        unsigned char randChar = (unsigned char)(rand * 0xff);
        QRgb col = (0xff << 24) | (randChar << 16) | (randChar << 8) | randChar;
        brush->setColor(QColor::fromRgb(col));

        for ( int i = 0; i < brush->brushFaceCount(); i++ )
        {
            GenericBrushFace* face = brush->brushFaceAt(i);
            face->texturePlane()->setTextureId(m_iPlaceholderMaterial);
        }
    }
}

void MainWindow::setVpkPath(const QString &path)
{
    m_strVpkPath = path;
    m_VpkFiles.clear();
    m_VpkFiles.addFilesFromDirectory(path);

    if ( !m_strVpkPath.isEmpty() )
    {
        qDebug() << "VPK path:" << m_strVpkPath;
    }
    else
    {
        qDebug() << "VPK path is empty.";
    }
}

void MainWindow::importTextures()
{
    ModelLoaders::VTFLoader::loadMaterials(m_VpkFiles, m_pMaterialStore, m_pTextureStore);
}
