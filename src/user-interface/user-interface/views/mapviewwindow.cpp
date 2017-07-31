#include "mapviewwindow.h"
#include <QMouseEvent>
#include <QMessageBox>
#include <QFile>
#include "file-formats/keyvalues/keyvaluesparser.h"
#include <QtDebug>
#include "model/genericbrush/genericbrush.h"
#include <QtGlobal>
#include <QMap>

#include "file-formats/vpk/vpkindextreerecord.h"

#include "model-loaders/vtf/vtfloader.h"
#include "model-loaders/filedataloaders/vmf/vmfdataloader.h"

#include "rendersystem/endpoints/rendermodelstoreendpoint.h"
#include "rendersystem/endpoints/framebufferstoreendpoint.h"

#include "calliperutil/opengl/openglerrors.h"
#include "calliperutil/opengl/openglhelpers.h"

using namespace Model;
using namespace RenderSystem;

namespace UserInterface
{
    MapViewWindow::MapViewWindow() :
        QOpenGLWindow(),
        m_strMapPath(),
        m_strVpkPath(),
        m_bInitialised(false),
        m_pVmfData(Q_NULLPTR),
        m_pCameraController(Q_NULLPTR),
        m_pKeyMap(Q_NULLPTR),
        m_pMouseEventMap(Q_NULLPTR),
        m_nRenderFrameBufferId(RenderSystem::FrameBufferDefs::INVALID_FRAME_BUFFER_ID),
        m_FrameBufferCopier()
    {
    }

    MapViewWindow::~MapViewWindow()
    {
        destroy();
    }

    void MapViewWindow::destroy()
    {
        delete m_pMouseEventMap;
        m_pMouseEventMap = Q_NULLPTR;

        delete m_pKeyMap;
        m_pKeyMap = Q_NULLPTR;

        delete m_pCameraController;
        m_pCameraController = Q_NULLPTR;

        delete m_pVmfData;
        m_pVmfData = Q_NULLPTR;

        destroyFrameBuffer();

        makeCurrent();
        m_FrameBufferCopier.destroy();
        doneCurrent();
    }

    void MapViewWindow::createFrameBuffer()
    {
        using namespace RenderSystem;

        Q_ASSERT_X(m_nRenderFrameBufferId == RenderSystem::FrameBufferDefs::INVALID_FRAME_BUFFER_ID,
                   Q_FUNC_INFO,
                   "Expected frame buffer to be uninitialised!");

        FrameBufferStoreEndpoint::FrameBufferStoreAccessor frameBufferStore = FrameBufferStoreEndpoint::frameBufferStore();
        m_nRenderFrameBufferId = frameBufferStore->createFrameBuffer(size());

        Q_ASSERT_X(m_nRenderFrameBufferId != RenderSystem::FrameBufferDefs::INVALID_FRAME_BUFFER_ID,
                   Q_FUNC_INFO,
                   "Expected valid frame buffer ID!");
    }

    void MapViewWindow::destroyFrameBuffer()
    {
        using namespace RenderSystem;

        if ( m_nRenderFrameBufferId != FrameBufferDefs::INVALID_FRAME_BUFFER_ID )
        {
            FrameBufferStoreEndpoint::FrameBufferStoreAccessor frameBufferStore = FrameBufferStoreEndpoint::frameBufferStore();
            frameBufferStore->removeFrameBuffer(m_nRenderFrameBufferId);
            m_nRenderFrameBufferId = FrameBufferDefs::INVALID_FRAME_BUFFER_ID;
        }
    }

    void MapViewWindow::initializeGL()
    {
        initLocalOpenGlSettings();

        initShaders();
        initTextures();
        initMaterials();

        m_pVmfData = new MapFileDataModel();

        m_pCameraController = new CameraController(this);
        initCameraController();

        m_pKeyMap = new KeyMap(this);
        initKeyMap();

        m_pMouseEventMap = new MouseEventMap(this);
        initMouseEventMap();

        m_FrameBufferCopier.create();
        createFrameBuffer();

        m_bInitialised = true;
        emit initialised();
    }

    void MapViewWindow::paintGL()
    {
        if ( !m_bInitialised || !m_pVmfData ||  m_pVmfData->renderModelId() == RenderSystem::RenderModelDefs::INVALID_RENDER_MODEL_ID )
        {
            return;
        }

        FrameDrawParams drawParams;
        drawParams.setWorldToCameraMatrix(m_pVmfData->scene()->defaultCamera()->rootToLocalMatrix());
        drawParams.setProjectionMatrix(m_pVmfData->scene()->defaultCamera()->lens().projectionMatrix());
        drawParams.setRenderMode(RenderSystem::ShaderDefs::BarebonesRenderMode);
        drawParams.setBackgroundColor(QColor::fromRgb(0xff040404));

        {
            RenderSystem::RenderModelStoreEndpoint::RenderModelStoreAccessor rms = RenderSystem::RenderModelStoreEndpoint::renderModelStore();
            rms->draw(m_pVmfData->renderModelId(), m_nRenderFrameBufferId, drawParams);
        }

        GL_CURRENT_F;

        f->glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        const GLuint textureId = RenderSystem::FrameBufferStoreEndpoint::constFrameBufferStore()
                ->frameBufferTextureId(m_nRenderFrameBufferId);

        m_FrameBufferCopier.draw(textureId);
    }

    void MapViewWindow::resizeGL(int w, int h)
    {
        if ( !m_bInitialised || !m_pVmfData )
        {
            return;
        }

        SceneCamera* const camera = m_pVmfData->scene()->defaultCamera();
        CameraLens lens = camera->lens();

        lens.setAspectRatio(static_cast<float>(w)/static_cast<float>(h));
        camera->setLens(lens);
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
    }

    void MapViewWindow::initScene()
    {

    }

    void MapViewWindow::initCameraController()
    {
        m_pCameraController->setCamera(m_pVmfData->scene()->defaultCamera());
        m_pCameraController->setStrafeSpeed(100.0f);
        m_pCameraController->setForwardSpeed(100.0f);
        m_pCameraController->setVerticalSpeed(100.0f);
        m_pCameraController->setEnabled(true);

        connect(m_pCameraController, &CameraController::tickFinished, this, [this]{ update(); });
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
        connect(m_pMouseEventMap, &MouseEventMap::mouseMovedX, this, [this]{ update(); });
        connect(m_pMouseEventMap, &MouseEventMap::mouseMovedY, this, [this]{ update(); });

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
        {
            return;
        }

        m_strVpkPath = path;
    }

    QString MapViewWindow::mapPath() const
    {
        return m_strMapPath;
    }

    void MapViewWindow::setMapPath(const QString& path)
    {
        if ( m_strMapPath == path )
        {
            return;
        }

        m_strMapPath = path;
    }

    const FileFormats::VPKFileCollection& MapViewWindow::vpkFileCollection() const
    {
        return m_VpkFiles;
    }

    Model::MapFileDataModel* MapViewWindow::mapDataModel()
    {
        return m_pVmfData;
    }

    const Model::MapFileDataModel* MapViewWindow::mapDataModel() const
    {
        return m_pVmfData;
    }

    void MapViewWindow::loadMap()
    {
        using namespace FileFormats;
        using namespace ModelLoaders;

        if ( m_strMapPath.isNull() || m_strMapPath.isEmpty() )
        {
            QMessageBox::critical(Q_NULLPTR, "Error", "No VMF file provided.");
            return;
        }

        VmfDataLoader loader;
        loader.setDataModel(m_pVmfData);
        QString errorString;

        switch ( loader.load(m_strMapPath, &errorString) )
        {
            case BaseFileLoader::Failure:
            {
                QMessageBox msg(QMessageBox::Critical, "Error", "Unable to load VMF.", QMessageBox::Ok);
                if ( !errorString.isEmpty() )
                {
                    msg.setDetailedText(errorString);
                }

                return;
            }

            case BaseFileLoader::PartialSuccess:
            {
                QMessageBox msg(QMessageBox::Warning, "Warning", "Some errors occurred when loading the VMF.", QMessageBox::Ok);
                if ( !errorString.isEmpty() )
                {
                    msg.setDetailedText(errorString);
                }

                break;
            }

            default:
            {
                break;
            }
        }

        m_pVmfData->scene()->updateRenderGeometry(m_pVmfData->renderModelId());
    }

    void MapViewWindow::loadVpks()
    {
        m_VpkFiles.clear();
        m_VpkFiles.addFilesFromDirectory(m_strVpkPath);
    }
}
