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
        m_pMouseEventMap(Q_NULLPTR)
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

        m_bInitialised = true;
        emit initialised();
    }

    void MapViewWindow::paintGL()
    {
        if ( !m_bInitialised )
        {
            return;
        }
    }

    void MapViewWindow::resizeGL(int w, int h)
    {
        if ( !m_bInitialised )
        {
            return;
        }
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

        if ( loader.load(m_strMapPath, &errorString) != BaseFileLoader::Success )
        {
            QMessageBox msg(QMessageBox::Critical, "Error", "Unable to load VMF.", QMessageBox::Ok);
            if ( !errorString.isEmpty() )
            {
                msg.setDetailedText(errorString);
            }
        }
    }

    void MapViewWindow::loadVpks()
    {
        m_VpkFiles.clear();
        m_VpkFiles.addFilesFromDirectory(m_strVpkPath);
    }
}
