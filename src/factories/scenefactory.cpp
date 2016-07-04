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
#include "textureplane.h"
#include "brushfactory.h"
#include "scalehandle.h"
#include "openglrenderer.h"
#include "basedraghandle.h"
#include "keyvaluesparser.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "texturedpolygon.h"
#include "generalutil.h"

namespace SceneFactory
{
    MapScene* defaultScene(MapDocument* document)
    {
        MapScene* scene = new MapScene(document);

        SceneCamera* c = scene->createSceneObject<SceneCamera>(scene->root());
        c->setObjectName("camera");
        c->setPosition(QVector3D(128, 128, 80));
        c->lookAtGlobal(QVector3D(0,0,0));

        return scene;
    }

    MapScene* debugScene(MapDocument *document)
    {
        MapScene* scene = new MapScene(document);

        SceneCamera* c = scene->createSceneObject<SceneCamera>(scene->root());
        c->setObjectName("camera");
        c->setPosition(QVector3D(128, 128, 80));
        c->lookAtGlobal(QVector3D(0,0,0));

        // For the purposes of debugging, serialise our geometry data.
        // This is pretty damn verbose but it means we can load it back in easily.

        SceneObject* camModel = scene->createSceneObject<SceneObject>(scene->root());
        camModel->setObjectName("camModel");
        camModel->appendGeometry(GeometryFactory::fromObjFile(":/models/editor/camera.obj", 32));
        camModel->setPosition(QVector3D(0,128,0));
        camModel->setShouldSerialiseGeometry(true);

        SceneObject* block = scene->createSceneObject<SceneObject>(scene->root());
        block->setObjectName("block");
        block->appendGeometry(GeometryFactory::cube(32.0f));
        block->geometryAt(0)->setTexture(0, "/textures/test");
        block->setPosition(QVector3D(64,0,0));
        block->setShouldSerialiseGeometry(true);

        SceneObject* blockChild = scene->cloneSceneObject<SceneObject>(block);
        blockChild->setObjectName("blockChild");
        blockChild->setParent(block);
        blockChild->clearGeometry();
        blockChild->appendGeometry(GeometryFactory::cube(8.0f));
        blockChild->geometryAt(0)->setTexture(0, "/textures/test");
        blockChild->setPosition(QVector3D(0,0,40));
        blockChild->setShouldSerialiseGeometry(true);

        SceneObject* block2 = scene->cloneSceneObject<SceneObject>(block);
        block2->setObjectName("block2");
        block2->geometryAt(0)->setTexture(0, "/textures/debug_translucent");
        block2->setPosition(QVector3D(128,0,0));
        block2->setRenderFlags(SceneObject::Translucent);
        block2->setShouldSerialiseGeometry(true);

        SceneObject* testBlock = scene->createSceneObject<SceneObject>(scene->root());
        testBlock->setObjectName("testBlock");
        testBlock->appendGeometry(GeometryFactory::cubeSolidColor(96.0f, QColor::fromRgba(0x80ff8800)));
        testBlock->setPosition(QVector3D(0,-256,0));
        testBlock->setRenderFlags(SceneObject::Translucent);
        testBlock->setShouldSerialiseGeometry(true);

        SceneObject* testBlock2 = scene->cloneSceneObject<SceneObject>(testBlock);
        testBlock2->setObjectName("testBlock2");
        testBlock2->setPosition(QVector3D(0, -512, 0));
        testBlock->setShouldSerialiseGeometry(true);

        Brush* b = BrushFactory::fromBoundingBox(scene, scene->root(), BoundingBox(QVector3D(0,0,0), QVector3D(256,256,256)), "/textures/test");
        b->setObjectName("brush");
        b->setPosition(QVector3D(256,0,0));

        SceneObject* arrow = scene->createSceneObject<SceneObject>(scene->root());
        arrow->appendGeometry(GeometryFactory::hexPin(5, 0.1f));
        arrow->setScale(QVector3D(16,16,16));
        arrow->setPosition(-renderer()->directionalLight() * 128);

        {
            QFile file(":/samples/maps/apartment_building.vmf");
            if ( file.open(QIODevice::ReadOnly) )
            {
                QByteArray data = file.readAll();
                file.close();

                KeyValuesParser parser(data);
                QJsonDocument doc = parser.toJsonDocument();
                QJsonObject world = doc.object().value("world").toObject();
                QJsonArray solids = world.value("solid").toArray();
                for ( int i = 0; i < solids.count(); i++ )
                {
                    QList<TexturedPolygon*> polygons;

                    QJsonObject solid = solids.at(i).toObject();
                    QJsonArray sides = solid.value("side").toArray();
                    for ( int j = 0; j < sides.count(); j++ )
                    {
                        QString plane = sides.at(j).toObject().value("plane").toString();
                        QVector3D v0, v1, v2;
                        GeneralUtil::vectorsFromVmfCoords(plane, v0, v1, v2);
                        polygons.append(new TexturedPolygon(Plane3D(v0, v1, v2), QString()));
                        Q_ASSERT(!QVector3D::crossProduct(v1 - v0, v2 - v0).isNull());
                    }

                    Brush* planeBrush = BrushFactory::fromPolygons(scene, scene->root(), polygons);
                    planeBrush->setObjectName(QString("planeBrush%0").arg(i));
                }
            }
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
