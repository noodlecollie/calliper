#include "scenefactory.h"
#include "mapscene.h"
#include "originmarker.h"
#include "scenecamera.h"
#include <QVector3D>
#include "mapdocument.h"
#include "sceneobject.h"
#include "geometryfactory.h"
#include "basegrid.h"
#include "callipermath.h"
#include <QtMath>
#include <QtDebug>

namespace SceneFactory
{
    MapScene* defaultScene(MapDocument* document)
    {
        MapScene* scene = new MapScene(document);

        OriginMarker* o = new OriginMarker(scene->root());
        o->setObjectName("origin");

        BaseGrid* grid = new BaseGrid(scene->root());
        grid->setObjectName("baseGrid");

        SceneCamera* c = new SceneCamera(scene->root());
        c->setObjectName("camera");
        c->setPosition(QVector3D(128, 128, 80));
        c->lookAt(QVector3D(0,0,0));

        return scene;
    }

    MapScene* debugScene(MapDocument *document)
    {
        MapScene* scene = new MapScene(document);

        OriginMarker* o = new OriginMarker(scene->root());
        o->setObjectName("origin");

        BaseGrid* grid = new BaseGrid(scene->root());
        grid->setObjectName("baseGrid");

        SceneCamera* c = new SceneCamera(scene->root());
        c->setObjectName("camera");
        c->setPosition(QVector3D(128, 128, 80));
        c->lookAt(QVector3D(0,0,0));

        c = new SceneCamera(scene->root());
        c->setObjectName("camera2");
        c->setPosition(QVector3D(128,0,0));
        c->lens()->setFieldOfView(45);
        c->lens()->setNearPlane(1);
        c->lens()->setFarPlane(100);
        c->setDrawBounds(true);

        SceneObject* block = new SceneObject(scene->root());
        block->setObjectName("block");
        block->setGeometry(GeometryFactory::cube(32.0f));
        block->geometry()->setTexture(0, "/textures/test");
        block->setPosition(QVector3D(64,0,0));

        SceneObject* camModel = new SceneObject(scene->root());
        camModel->setObjectName("camModel");
        camModel->setGeometry(GeometryFactory::fromObjFile(":/models/editor/camera.obj", 32));
        camModel->setPosition(QVector3D(0,128,0));

        return scene;
    }
}
