#ifndef MAPVIEWPORT_H
#define MAPVIEWPORT_H

#include "user-interface_global.h"
#include <QOpenGLWidget>
#include "model/filedatamodels/map/mapfiledatamodel.h"
#include <QSharedPointer>
#include "user-interface/modelviews/imodelview.h"

namespace UserInterface
{
    class MapViewport : public QOpenGLWidget, public IModelView
    {
        Q_OBJECT
    public:
        MapViewport(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

        virtual void loadDataModel(const QSharedPointer<Model::BaseFileDataModel> &model) override;

    signals:

    public slots:

    protected:
        virtual void initializeGL() override;
        virtual void paintGL() override;
        virtual void resizeGL(int w, int h) override;

    private:
        QSharedPointer<Model::MapFileDataModel> m_pDataModel;
    };
}

#endif // MAPVIEWPORT_H
