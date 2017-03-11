#ifndef MAPVIEWPORT_H
#define MAPVIEWPORT_H

#include "user-interface_global.h"
#include <QOpenGLWidget>
#include "model/scene/mapscene.h"
#include "model/scenerenderer/scenerenderer.h"
#include <QPointer>

namespace UserInterface
{
    class MapViewport : public QOpenGLWidget
    {
        Q_OBJECT
    public:
        MapViewport(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

        Model::MapScene* scene() const;
        void setScene(Model::MapScene* scene);

    signals:

    public slots:

    private:
        QPointer<Model::MapScene> m_pScene;
    };
}

#endif // MAPVIEWPORT_H
