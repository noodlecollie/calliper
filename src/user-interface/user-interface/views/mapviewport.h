#ifndef MAPVIEWPORT_H
#define MAPVIEWPORT_H

#include "user-interface_global.h"
#include <QOpenGLWidget>
#include "model/filedatamodels/map/mapfiledatamodel.h"
#include <QSharedPointer>
#include "user-interface/modelviews/imodelview.h"

namespace Model
{
    class CameraController;
    class KeyMap;
    class MouseEventMap;
}

namespace UserInterface
{
    class MapViewport : public QOpenGLWidget, public IModelView
    {
        Q_OBJECT
    public:
        MapViewport(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
        virtual ~MapViewport();

        virtual QWidget* modelViewToWidget() override;
        virtual const QWidget* modelViewToWidget() const override;

        virtual QWeakPointer<Model::BaseFileDataModel> dataModel() const override;
        virtual void setDataModel(const QWeakPointer<Model::BaseFileDataModel> &model) override;

    signals:

    public slots:

    protected:
        typedef QWeakPointer<Model::MapFileDataModel> MapFileDataModelWeakRef;
        typedef QSharedPointer<Model::MapFileDataModel> MapFileDataModelPointer;

        virtual void initializeGL() override;
        virtual void paintGL() override;
        virtual void resizeGL(int w, int h) override;

        MapFileDataModelPointer mapDataModel();

    private slots:
        void handleMouseMovedX(float amount);
        void handleMouseMovedY(float amount);

    private:
        typedef void (Model::CameraController::* CameraControlSlot)(bool);

        void initKeyMap();
        void initMouseEventMap();
        void connectCameraControl(Qt::Key key, CameraControlSlot slot);

        MapFileDataModelWeakRef m_pDataModel;

        Model::CameraController* m_pCameraController;
        Model::KeyMap* m_pKeyMap;
        Model::MouseEventMap* m_pMouseEventMap;
    };
}

#endif // MAPVIEWPORT_H
