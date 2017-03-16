#include "mapviewport.h"

#include "renderer/opengl/openglhelpers.h"
#include "renderer/opengl/openglerrors.h"

#include "model/global/resourceenvironment.h"
#include "model/scenerenderer/scenerenderer.h"
#include "model/controller-adapters/keymap.h"
#include "model/camera/cameracontroller.h"
#include "model/controller-adapters/mouseeventmap.h"

namespace
{
    const Qt::Key FORWARD_KEY_DEFAULT = Qt::Key_W;
    const Qt::Key BACKWARD_KEY_DEFAULT = Qt::Key_S;
    const Qt::Key LEFT_KEY_DEFAULT = Qt::Key_A;
    const Qt::Key RIGHT_KEY_DEFAULT = Qt::Key_D;
    const Qt::Key UP_KEY_DEFAULT = Qt::Key_Q;
    const Qt::Key DOWN_KEY_DEFAULT = Qt::Key_Z;
}

namespace UserInterface
{
    MapViewport::MapViewport(QWidget *parent, Qt::WindowFlags f)
        : QOpenGLWidget(parent, f),
          m_pDataModel(),
          m_pCameraController(new Model::CameraController(this)),
          m_pKeyMap(new Model::KeyMap(this)),
          m_pMouseEventMap(new Model::MouseEventMap(this))
    {
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
    }

    MapViewport::MapFileDataModelPointer MapViewport::mapDataModel()
    {
        return m_pDataModel.toStrongRef();
    }

    void MapViewport::initKeyMap()
    {
        using namespace Model;

        installEventFilter(m_pKeyMap);

        connectCameraControl(FORWARD_KEY_DEFAULT, &CameraController::moveForward);
        connectCameraControl(BACKWARD_KEY_DEFAULT, &CameraController::moveBackward);
        connectCameraControl(LEFT_KEY_DEFAULT, &CameraController::moveLeft);
        connectCameraControl(RIGHT_KEY_DEFAULT, &CameraController::moveRight);
        connectCameraControl(UP_KEY_DEFAULT, &CameraController::moveUp);
        connectCameraControl(DOWN_KEY_DEFAULT, &CameraController::moveDown);
    }

    void MapViewport::connectCameraControl(Qt::Key key, CameraControlSlot slot)
    {
        connect(m_pKeyMap->addKeyMap(key), &Model::KeySignalSender::keyEvent, m_pCameraController, slot);
    }

    void MapViewport::initMouseEventMap()
    {
        using namespace Model;

        installEventFilter(m_pMouseEventMap);

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
        GL_CURRENT_F;

        GLTRY(f->glEnable(GL_CULL_FACE));
        GLTRY(f->glCullFace(GL_BACK));

        GLTRY(f->glEnable(GL_DEPTH_TEST));
        GLTRY(f->glDepthFunc(GL_LESS));
    }

    void MapViewport::paintGL()
    {
        MapFileDataModelPointer mapModel = m_pDataModel.toStrongRef();
        if ( !mapModel )
        {
            return;
        }

        Model::SceneRenderer sceneRenderer(mapModel->scene(), mapModel->renderModel());
        sceneRenderer.setShaderPalette(Model::ResourceEnvironment::globalInstance()->shaderPaletteStore()
                                       ->shaderPalette(Model::ShaderPaletteStore::SimpleLitTexturedRenderMode));

        GL_CURRENT_F;
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
}
