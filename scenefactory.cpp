#include "scenefactory.h"
#include "scene.h"
#include "originmarker.h"
#include "camera.h"
#include <QVector3D>

namespace SceneFactory
{
    Scene* defaultScene()
    {
        Scene* scene = new Scene();

        new OriginMarker(scene->root());

        Camera* c = new Camera(scene->root());
        c->setPosition(QVector3D(0, -128, 64));
        c->lookAt(QVector3D(0,0,0));

        return scene;
    }
}
