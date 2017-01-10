#include "mapviewwindow.h"
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
#include <QtGlobal>
#include <QMap>
#include "vpk/vpkindextreerecord.h"
#include "vtf/vtfloader.h"

using namespace Model;
using namespace Renderer;

namespace UserInterface
{
    MapViewWindow::MapViewWindow(IRenderPassClassifier *classifier) :
        QOpenGLWindow(),
        m_strMapPath(),
        m_strVpkPath(),
        m_pRenderer(nullptr),
        m_pShaderStore(nullptr),
        m_pTextureStore(nullptr),
        m_pMaterialStore(nullptr),
        m_pScene(nullptr),
        m_pCamera(nullptr),
        m_pRenderPassClassifier(classifier),
        m_pSceneRenderer(nullptr),
        m_ShaderPalette(),
        m_pCameraController(nullptr),
        m_pKeyMap(nullptr),
        m_pMouseEventMap(nullptr)
    {
    }

    MapViewWindow::~MapViewWindow()
    {
        makeCurrent();
        destroy();
        doneCurrent();
    }

    void MapViewWindow::destroy()
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

        delete m_pRenderer;
        m_pRenderer = nullptr;

        delete m_pShaderStore;
        m_pShaderStore = nullptr;

        delete m_pTextureStore;
        m_pTextureStore = nullptr;

        delete m_pMaterialStore;
        m_pMaterialStore = nullptr;

        delete m_pRenderPassClassifier;
        m_pRenderPassClassifier = nullptr;
    }

    void MapViewWindow::initializeGL()
    {
        initLocalOpenGlSettings();

        m_pShaderStore = new ShaderStore();
        initShaders();

        m_pTextureStore = new TextureStore();
        initTextures();

        m_pMaterialStore = new MaterialStore();
        initMaterials();

        m_pRenderer = new Renderer::RenderModel();
        initRenderer();

        m_pScene = new BasicScene(m_pShaderStore, m_pTextureStore, m_pMaterialStore, this);
        initScene();

        m_pSceneRenderer = new SceneRenderer(m_pShaderStore, m_pTextureStore, m_pMaterialStore, m_pRenderPassClassifier,
                                             m_pRenderer, m_pScene);
        initSceneRenderer();

        m_pCameraController = new CameraController(this);
        initCameraController();

        m_pKeyMap = new KeyMap(this);
        initKeyMap();

        m_pMouseEventMap = new MouseEventMap(this);
        initMouseEventMap();
    }

    void MapViewWindow::paintGL()
    {
        m_pSceneRenderer->setShaderPalette(m_ShaderPalette);

        GL_CURRENT_F;
        GLTRY(f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        m_pSceneRenderer->render(m_pCamera);
    }

    void MapViewWindow::resizeGL(int w, int h)
    {
        if ( !m_pCamera )
            return;

        CameraLens lens = m_pCamera->lens();
        lens.setAspectRatio(static_cast<float>(w)/static_cast<float>(h));
        m_pCamera->setLens(lens);
    }

    void MapViewWindow::initShaders()
    {
    }

    void MapViewWindow::initTextures()
    {
    }

    void MapViewWindow::initMaterials()
    {
    }

    void MapViewWindow::initLocalOpenGlSettings()
    {
    }

    void MapViewWindow::initRenderer()
    {
        m_pRenderer->setShaderFunctor(m_pShaderStore);
        m_pRenderer->setTextureFunctor(m_pTextureStore);
        m_pRenderer->setMaterialFunctor(m_pMaterialStore);
    }

    void MapViewWindow::initScene()
    {
        m_pCamera = m_pScene->defaultCamera();
    }

    void MapViewWindow::initSceneRenderer()
    {

    }

    void MapViewWindow::initCameraController()
    {
        m_pCameraController->setCamera(m_pCamera);
        m_pCameraController->setStrafeSpeed(100.0f);
        m_pCameraController->setForwardSpeed(100.0f);
        m_pCameraController->setVerticalSpeed(100.0f);
        m_pCameraController->setEnabled(true);

        connect(m_pCameraController, SIGNAL(tickFinished()), this, SLOT(update()));
    }

    void MapViewWindow::initKeyMap()
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
                this, &MapViewWindow::close);

        installEventFilter(m_pKeyMap);
    }

    void MapViewWindow::initMouseEventMap()
    {
        m_pMouseEventMap->setHorizontalSensitivity(-1);
        connect(m_pMouseEventMap, &MouseEventMap::mouseMovedX, m_pCameraController, &CameraController::addYaw);
        connect(m_pMouseEventMap, &MouseEventMap::mouseMovedY, m_pCameraController, &CameraController::addPitch);
        connect(m_pMouseEventMap, SIGNAL(mouseMovedX(float)), this, SLOT(update()));
        connect(m_pMouseEventMap, SIGNAL(mouseMovedY(float)), this, SLOT(update()));

        installEventFilter(m_pMouseEventMap);
    }

    void MapViewWindow::mousePressEvent(QMouseEvent *e)
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

    void MapViewWindow::mouseReleaseEvent(QMouseEvent *e)
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

    QString MapViewWindow::vpkPath() const
    {
        return m_strVpkPath;
    }

    void MapViewWindow::setVpkPath(const QString& path)
    {
        if ( m_strVpkPath == path )
            return;

        m_strVpkPath = path;
    }

    QString MapViewWindow::mapPath() const
    {
        return m_strMapPath;
    }

    void MapViewWindow::setMapPath(const QString& path)
    {
        if ( m_strMapPath == path )
            return;

        m_strMapPath = path;
    }

    Model::ShaderPalette& MapViewWindow::shaderPalette()
    {
        return m_ShaderPalette;
    }

    const Model::ShaderPalette& MapViewWindow::shaderPalette() const
    {
        return m_ShaderPalette;
    }

    Model::ShaderStore* MapViewWindow::shaderStore()
    {
        return m_pShaderStore;
    }

    const Model::ShaderStore* MapViewWindow::shaderStore() const
    {
        return m_pShaderStore;
    }

    Model::TextureStore* MapViewWindow::textureStore()
    {
        return m_pTextureStore;
    }

    const Model::TextureStore* MapViewWindow::textureStore() const
    {
        return m_pTextureStore;
    }

    Model::MaterialStore* MapViewWindow::materialStore()
    {
        return m_pMaterialStore;
    }

    const Model::MaterialStore* MapViewWindow::materialStore() const
    {
        return m_pMaterialStore;
    }

    Model::IRenderPassClassifier* MapViewWindow::renderPassClassifier()
    {
        return m_pRenderPassClassifier;
    }

    const Model::IRenderPassClassifier* MapViewWindow::renderPassClassifier() const
    {
        return m_pRenderPassClassifier;
    }

    Model::Scene* MapViewWindow::scene()
    {
        return m_pScene;
    }

    const Model::Scene* MapViewWindow::scene() const
    {
        return m_pScene;
    }

    Model::SceneCamera* MapViewWindow::sceneCamera()
    {
        return m_pCamera;
    }

    const Model::SceneCamera* MapViewWindow::sceneCamera() const
    {
        return m_pCamera;
    }

    const FileFormats::VPKFileCollection& MapViewWindow::vpkFileCollection() const
    {
        return m_VpkFiles;
    }

    void MapViewWindow::loadMap()
    {
        using namespace FileFormats;
        using namespace ModelLoaders;

        if ( m_strMapPath.isNull() || m_strMapPath.isEmpty() )
        {
            QMessageBox::critical(nullptr, "Error", "No VMF file provided.");
            return;
        }

        QFile file(m_strMapPath);
        if ( !file.open(QIODevice::ReadOnly) )
        {
            QMessageBox::critical(nullptr, "Error", QString("Could not open VMF file ") + m_strMapPath);
            return;
        }

        QByteArray fileData = file.readAll();
        file.close();

        QJsonDocument vmfDoc;

        {
            KeyValuesParser kvParser(fileData);
            QString err;
            vmfDoc = kvParser.toJsonDocument(&err);
            if ( vmfDoc.isNull() )
                return;
        }

        VMFLoader::createBrushes(vmfDoc, m_pScene->rootObject());
    }

    void MapViewWindow::loadVpks()
    {
        m_VpkFiles.clear();
        m_VpkFiles.addFilesFromDirectory(m_strVpkPath);
    }
}