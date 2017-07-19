#ifndef MAPVIEWPORT_H
#define MAPVIEWPORT_H

#include "user-interface_global.h"

#include <QOpenGLWidget>
#include <QSharedPointer>

#include "user-interface/opengl/framebuffercopier.h"
#include "user-interface/modelviews/imodelview.h"

#include "rendersystem/interface-classes/definitions/framebufferdefs.h"

#include "model/filedatamodels/map/mapfiledatamodel.h"

#include "calliperutil/opengl/openglcontextchecker.h"

namespace Model
{
    class CameraController;
    class KeyMap;
    class MouseEventMap;
}

namespace UserInterface
{
    class MapViewport : public QOpenGLWidget,
                        public IModelView,
                        public CalliperUtil::OpenGLContextChecker
    {
        Q_OBJECT
    public:
        MapViewport(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
        virtual ~MapViewport();

        virtual QWidget* modelViewToWidget() override;
        virtual const QWidget* modelViewToWidget() const override;

        virtual QWeakPointer<Model::BaseFileDataModel> dataModel() const override;
        virtual void setDataModel(const QWeakPointer<Model::BaseFileDataModel> &model) override;

        bool mouseLookEnabled() const;

    signals:

    public slots:
        void toggleMouseLookEnabled();
        void setMouseLookEnabled(bool enabled);

    protected:
        typedef QWeakPointer<Model::MapFileDataModel> MapFileDataModelWeakRef;
        typedef QSharedPointer<Model::MapFileDataModel> MapFileDataModelPointer;

        virtual void focusInEvent(QFocusEvent *event) override;
        virtual void focusOutEvent(QFocusEvent *event) override;

        virtual void initializeGL() override;
        virtual void paintGL() override;
        virtual void resizeGL(int w, int h) override;

        MapFileDataModelPointer mapDataModel();

    private slots:
        void handleMouseMovedX(float amount);
        void handleMouseMovedY(float amount);

    private:
        typedef void (Model::CameraController::* CameraControlSlot)(bool);

        void initCameraController();
        void initKeyMap();
        void initMouseEventMap();
        void connectCameraControl(Qt::Key key, CameraControlSlot slot);
        bool drawRenderModel();
        void tempSaveFrameBufferOnMouseToggle();
        void destroyFrameBuffer();
        void checkRenderSystemContextIsNotCurrent();
        void updateFrameBufferId();
        void resizeFrameBuffer(const QSize& size);

        MapFileDataModelWeakRef m_pDataModel;

        bool m_bOpenGLInitialised;
        Model::CameraController* m_pCameraController;
        Model::KeyMap* m_pKeyMap;
        Model::MouseEventMap* m_pMouseEventMap;
        RenderSystem::FrameBufferDefs::FrameBufferId m_nRenderFrameBufferId;
        FrameBufferCopier m_FrameBufferCopier;
    };
}

#endif // MAPVIEWPORT_H
