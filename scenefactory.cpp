#include "scenefactory.h"
#include "scene.h"
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
    Scene* defaultScene(MapDocument* document)
    {
        Scene* scene = new Scene(document);

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

    Scene* debugScene(MapDocument *document)
    {
        Scene* scene = new Scene(document);

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
        trHandle->setIgnoreDepth(true);
        trHandle->setGeometry(GeometryFactory::translationHandle(64, QColor::fromRgb(0xffff0000)));
        GeometryData* g  = GeometryFactory::translationHandle(64, QColor::fromRgb(0xff00ff00),
                                                              Math::matrixRotateZ(qDegreesToRadians(90.0f)));
        trHandle->geometry()->append(*g);
        delete g;
        g  = GeometryFactory::translationHandle(64, QColor::fromRgb(0xff0000ff),
                                                Math::matrixRotateY(qDegreesToRadians(-90.0f)));
        trHandle->geometry()->append(*g);
        delete g;

        trHandle->setPosition(QVector3D(64,64,0));

        SceneObject* camModel = new SceneObject(scene->root());
        camModel->setObjectName("camModel");
        camModel->setGeometry(GeometryFactory::fromObjFile(":/models/editor/camera.obj", 32));
        camModel->setPosition(QVector3D(0,128,0));

        return scene;
    }
}
