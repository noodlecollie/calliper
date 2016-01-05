#include "scenefactory.h"
#include "scene.h"
#include "originmarker.h"
#include "camera.h"
#include <QVector3D>
#include "mapdocument.h"
#include "sceneobject.h"
#include "geometryfactory.h"
#include "basegrid.h"

namespace SceneFactory
{
    Scene* defaultScene(MapDocument* document)
    {
        Scene* scene = new Scene(document);

        OriginMarker* o = new OriginMarker(scene->root());
        o->setObjectName("origin");

        BaseGrid* grid = new BaseGrid(scene->root());
        grid->setObjectName("baseGrid");

        Camera* c = new Camera(scene->root());
        c->setObjectName("camera");
        c->setPosition(QVector3D(128, 128, 80));
        c->lookAt(QVector3D(0,0,0));

        return scene;
    }

    Scene* debugScene(MapDocument *document)
    {
        Scene* scene = new Scene(document);

        OriginMarker* o = new OriginMarker(scene->root());
        o->setObjectName("origin");

        BaseGrid* grid = new BaseGrid(scene->root());
        grid->setObjectName("baseGrid");

        Camera* c = new Camera(scene->root());
        c->setObjectName("camera");
        c->setPosition(QVector3D(128, 128, 80));
        c->lookAt(QVector3D(0,0,0));

        c = new Camera(scene->root());
        c->setObjectName("camera2");
        c->setPosition(QVector3D(128,0,0));
        CameraLens lens = c->lens();
        lens.setFieldOfView(45);
        lens.setNearPlane(1);
        lens.setFarPlane(100);
        c->setLens(lens);
        c->setDrawBounds(true);

        SceneObject* block = new SceneObject(scene->root());
        block->setObjectName("block");
        block->setGeometry(GeometryFactory::cube(32.0f));
        block->geometry()->setTexture(0, "/textures/test");
        block->setPosition(QVector3D(64,0,0));

        SceneObject* trHandle = new SceneObject(scene->root());
        trHandle->setObjectName("translationHandle");
        trHandle->setGeometry(GeometryFactory::translationHandle(QColor::fromRgb(0xff00ff00)));
        trHandle->setPosition(QVector3D(64,64,0));
        trHandle->setScale(QVector3D(64,64,64));

        return scene;
    }
}
