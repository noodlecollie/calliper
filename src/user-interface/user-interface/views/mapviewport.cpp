#include "mapviewport.h"

#include <QFocusEvent>

#include "renderer/opengl/openglhelpers.h"
#include "renderer/opengl/openglerrors.h"

#include "model/global/resourceenvironment.h"
#include "model/scenerenderer/scenerenderer.h"
#include "model/controller-adapters/keymap.h"
#include "model/camera/cameracontroller.h"
#include "model/controller-adapters/mouseeventmap.h"

namespace
{
    const Qt::Key KEY_FORWARD_DEFAULT = Qt::Key_W;
    const Qt::Key KEY_BACKWARD_DEFAULT = Qt::Key_S;
    const Qt::Key KEY_LEFT_DEFAULT = Qt::Key_A;
    const Qt::Key KEY_RIGHT_DEFAULT = Qt::Key_D;
    const Qt::Key KEY_UP_DEFAULT = Qt::Key_Q;
    const Qt::Key KEY_DOWN_DEFAULT = Qt::Key_Z;
    const Qt::Key KEY_MOUSELOOK_TOGGLE_DEFAULT = Qt::Key_Space;
}

namespace UserInterface
{
    MapViewport::MapViewport(QWidget *parent, Qt::WindowFlags f)
        : QOpenGLWidget(parent, f),
          m_pDataModel(),
          m_bOpenGLInitialised(false),
          m_pCameraController(new Model::CameraController(this)),
          m_pKeyMap(new Model::KeyMap(this)),
          m_pMouseEventMap(new Model::MouseEventMap(this))
    {
        setFocusPolicy(Qt::StrongFocus);
        setMouseTracking(true);

        initCameraController();
        initKeyMap();
        initMouseEventMap();
    }

    MapViewport::~MapViewport()
    {
    }

    QWidget* MapViewport::modelViewToWidget()
    {
        return this;
    }

    const QWidget* MapViewport::modelViewToWidget() const
    {
        return this;
    }

    QWeakPointer<Model::BaseFileDataModel> MapViewport::dataModel() const
    {
        return m_pDataModel;
    }

    void MapViewport::setDataModel(const QWeakPointer<Model::BaseFileDataModel> &model)
    {
        QSharedPointer<Model::MapFileDataModel> mapModel = qSharedPointerDynamicCast<Model::MapFileDataModel>(model);
        if ( mapModel.isNull() )
        {
            m_pDataModel = QWeakPointer<Model::MapFileDataModel>();
        }

        m_pDataModel = mapModel.toWeakRef();
        m_pCameraController->setCamera(mapModel->scene()->defaultCamera());
    }

    MapViewport::MapFileDataModelPointer MapViewport::mapDataModel()
    {
        return m_pDataModel.toStrongRef();
    }

    void MapViewport::initCameraController()
    {
        // Camera controller is enabled on focus.
        m_pCameraController->setEnabled(false);

        m_pCameraController->setStrafeSpeed(100.0f);
        m_pCameraController->setForwardSpeed(100.0f);
        m_pCameraController->setVerticalSpeed(100.0f);

        connect(m_pCameraController, &Model::CameraController::tickFinished, this, [this]{ update(); });
    }

    void MapViewport::initKeyMap()
    {
        using namespace Model;

        installEventFilter(m_pKeyMap);

        connectCameraControl(KEY_FORWARD_DEFAULT, &CameraController::moveForward);
        connectCameraControl(KEY_BACKWARD_DEFAULT, &CameraController::moveBackward);
        connectCameraControl(KEY_LEFT_DEFAULT, &CameraController::moveLeft);
        connectCameraControl(KEY_RIGHT_DEFAULT, &CameraController::moveRight);
        connectCameraControl(KEY_UP_DEFAULT, &CameraController::moveUp);
        connectCameraControl(KEY_DOWN_DEFAULT, &CameraController::moveDown);

        connect(m_pKeyMap->addKeyMap(KEY_MOUSELOOK_TOGGLE_DEFAULT, Model::KeySignalSender::KeyPress), &Model::KeySignalSender::keyEvent,
                this, &MapViewport::toggleMouseLookEnabled);
    }

    void MapViewport::connectCameraControl(Qt::Key key, CameraControlSlot slot)
    {
        connect(m_pKeyMap->addKeyMap(key), &Model::KeySignalSender::keyEvent, m_pCameraController, slot);
    }

    void MapViewport::initMouseEventMap()
    {
        using namespace Model;

        installEventFilter(m_pMouseEventMap);

        m_pMouseEventMap->setShouldResetMouse(true);

        connect(m_pMouseEventMap, &MouseEventMap::mouseMovedX, this, &MapViewport::handleMouseMovedX);
        connect(m_pMouseEventMap, &MouseEventMap::mouseMovedY, this, &MapViewport::handleMouseMovedY);
    }

    void MapViewport::handleMouseMovedX(float amount)
    {
        m_pCameraController->addYaw(-amount);
        update();
    }

    void MapViewport::handleMouseMovedY(float amount)
    {
        m_pCameraController->addPitch(amount);
        update();
    }

    void MapViewport::initializeGL()
    {
        GL_MAIN_F;

        GLTRY(f->glEnable(GL_CULL_FACE));
        GLTRY(f->glCullFace(GL_BACK));

        GLTRY(f->glEnable(GL_DEPTH_TEST));
        GLTRY(f->glDepthFunc(GL_LESS));

        m_bOpenGLInitialised = true;
    }

    void MapViewport::paintGL()
    {
        if ( !m_bOpenGLInitialised )
        {
            return;
        }

        MapFileDataModelPointer mapModel = m_pDataModel.toStrongRef();
        if ( !mapModel )
        {
            return;
        }

        Model::SceneRenderer sceneRenderer(mapModel->scene(), mapModel->renderModel());
        sceneRenderer.setShaderPalette(Model::ResourceEnvironment::globalInstance()->shaderPaletteStore()
                                       ->shaderPalette(Model::ShaderPaletteStore::SimpleLitTexturedRenderMode));

        GL_MAIN_F;
        GLTRY(f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        sceneRenderer.render(mapModel->scene()->defaultCamera());
    }

    void MapViewport::resizeGL(int w, int h)
    {
        MapFileDataModelPointer mapModel = m_pDataModel.toStrongRef();
        if ( !mapModel )
        {
            return;
        }

        Model::SceneCamera* camera = mapModel->scene()->defaultCamera();

        Model::CameraLens lens = camera->lens();
        lens.setAspectRatio(static_cast<float>(w)/static_cast<float>(h));
        camera->setLens(lens);
    }

    void MapViewport::focusInEvent(QFocusEvent *event)
    {
        QOpenGLWidget::focusInEvent(event);

        m_pCameraController->setEnabled(true);
    }

    void MapViewport::focusOutEvent(QFocusEvent *event)
    {
        QOpenGLWidget::focusOutEvent(event);

        m_pCameraController->setEnabled(false);
        setMouseLookEnabled(false);
    }

    bool MapViewport::mouseLookEnabled() const
    {
        return m_pMouseEventMap->enabled();
    }

    void MapViewport::setMouseLookEnabled(bool enabled)
    {
        m_pMouseEventMap->setEnabled(enabled);

        if ( enabled )
        {
            grabMouse();
            setCursor(Qt::BlankCursor);
        }
        else
        {
            releaseMouse();
            unsetCursor();
        }
    }

    void MapViewport::toggleMouseLookEnabled()
    {
        setMouseLookEnabled(!mouseLookEnabled());
    }
}
