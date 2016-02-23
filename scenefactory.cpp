#include "scenefactory.h"
#include "mapscene.h"
#include "originmarker.h"
#include "scenecamera.h"
#include <QVector3D>
#include "mapdocument.h"
#include "sceneobject.h"
#include "geometryfactory.h"
#include "mapgrid.h"
#include "callipermath.h"
#include <QtMath>
#include <QtDebug>
#include "uiscene.h"
#include "translationhandle.h"
#include "brush.h"
#include "brushface.h"

namespace SceneFactory
{
    MapScene* defaultScene(MapDocument* document)
    {
        MapScene* scene = new MapScene(document);

        SceneCamera* c = new SceneCamera(scene->root());
        c->setObjectName("camera");
        c->setPosition(QVector3D(128, 128, 80));
        c->lookAt(QVector3D(0,0,0));

        return scene;
    }

    MapScene* debugScene(MapDocument *document)
    {
        MapScene* scene = new MapScene(document);

        SceneCamera* c = new SceneCamera(scene->root());
        c->setObjectName("camera");
        c->setPosition(QVector3D(128, 128, 80));
        c->lookAt(QVector3D(0,0,0));

        // For the purposes of debugging, serialise our geometry data.
        // This is pretty damn verbose but it means we can load it back in easily.

        SceneObject* camModel = new SceneObject(scene->root());
        camModel->setObjectName("camModel");
        camModel->setGeometry(GeometryFactory::fromObjFile(":/models/editor/camera.obj", 32));
        camModel->setPosition(QVector3D(0,128,0));
        camModel->setShouldSerialiseGeometry(true);

        SceneObject* block = new SceneObject(scene->root());
        block->setObjectName("block");
        block->setGeometry(GeometryFactory::cube(32.0f));
        block->geometry()->setTexture(0, "/textures/test");
        block->setPosition(QVector3D(64,0,0));
        block->setShouldSerialiseGeometry(true);

        SceneObject* blockChild = block->clone();
        blockChild->setObjectName("blockChild");
        blockChild->setParent(block);
        blockChild->setGeometry(GeometryFactory::cube(8.0f));
        blockChild->geometry()->setTexture(0, "/textures/test");
        blockChild->setPosition(QVector3D(0,0,40));
        blockChild->setShouldSerialiseGeometry(true);

        SceneObject* block2 = block->clone();
        block2->setObjectName("block2");
        block2->geometry()->setTexture(0, "/textures/debug_translucent");
        block2->setPosition(QVector3D(128,0,0));
        block2->setRenderFlags(SceneObject::Translucent);
        block2->setShouldSerialiseGeometry(true);

        SceneObject* testBlock = new SceneObject(scene->root());
        testBlock->setObjectName("testBlock");
        testBlock->setGeometry(GeometryFactory::cubeSolidColor(96.0f, QColor::fromRgba(0x80ff8800)));
        testBlock->setPosition(QVector3D(0,-256,0));
        testBlock->setRenderFlags(SceneObject::Translucent);
        testBlock->setShouldSerialiseGeometry(true);

        SceneObject* testBlock2 = testBlock->clone();
        testBlock2->setObjectName("testBlock2");
        testBlock2->setPosition(QVector3D(0, -512, 0));
        testBlock->setShouldSerialiseGeometry(true);

        Brush* b = new Brush(scene->root());
        b->setObjectName("brush");
        b->setPosition(QVector3D(256,0,0));

        b->appendVertex(QVector3D(-32,0,0));
        b->appendVertex(QVector3D(32,0,0));
        b->appendVertex(QVector3D(0,64,0));
        b->appendVertex(QVector3D(0,0,64));

        {
            BrushFace* f = new BrushFace(b);
            f->setObjectName("brushFace1");
            f->appendVertex(0);
            f->appendVertex(2);
            f->appendVertex(1);
        }

        {
            BrushFace* f = new BrushFace(b);
            f->setObjectName("brushFace2");
            f->appendVertex(0);
            f->appendVertex(3);
            f->appendVertex(2);
        }

        {
            BrushFace* f = new BrushFace(b);
            f->setObjectName("brushFace3");
            f->appendVertex(1);
            f->appendVertex(2);
            f->appendVertex(3);
        }

        {
            BrushFace* f = new BrushFace(b);
            f->setObjectName("brushFace4");
            f->appendVertex(0);
            f->appendVertex(1);
            f->appendVertex(3);
        }

        return scene;
    }

    UIScene* defaultUIScene(MapDocument *document)
    {
        UIScene* scene = new UIScene(document);

        return scene;
    }

    UIScene* debugUIScene(MapDocument *document)
    {
        UIScene* scene = new UIScene(document);

        return scene;
    }
}
