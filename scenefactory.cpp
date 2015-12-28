#include "scenefactory.h"
#include "scene.h"
#include "originmarker.h"
#include "camera.h"
#include <QVector3D>
#include "mapdocument.h"
#include "sceneobject.h"
#include "geometryfactory.h"

namespace SceneFactory
{
    Scene* defaultScene(MapDocument* document)
    {
        Scene* scene = new Scene(document);

//        OriginMarker* o = new OriginMarker(scene->root());
//        o->setObjectName("origin");

        Camera* c = new Camera(scene->root());
        c->setObjectName("camera");
        c->setPosition(QVector3D(0, -128, 64));
        c->lookAt(QVector3D(0,0,0));

        // TODO: Remove me!
        SceneObject* block = new SceneObject(scene->root());
        block->setObjectName("block");
        block->setGeometry(GeometryFactory::cube(32.0f));
        block->setPosition(QVector3D(64, 0, 0));
        block->setAngles(EulerAngle(0,45,0));
        block->geometry()->setTexture(0, "/textures/test");

        return scene;
    }
}
