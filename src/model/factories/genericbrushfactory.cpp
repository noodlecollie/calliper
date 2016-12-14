#include "genericbrushfactory.h"
#include "math/modelmath.h"
#include "scene/scene.h"

namespace Model
{
    namespace GenericBrushFactory
    {
        GenericBrush* createBrushFromWindingGroup(SceneObject* parent, QList<TexturedWinding*>& windings)
        {
            Q_ASSERT_X(parent, Q_FUNC_INFO, "Parent object must be provided!");

            Scene* scene = parent->parentScene();
            Q_ASSERT_X(scene, Q_FUNC_INFO, "Scene must be valid!");

            ModelMath::clipWindingsWithEachOther<TexturedWinding>(windings);
            QList<QVector3D> vertices = ModelMath::windingsToVertices<TexturedWinding>(windings);

            GenericBrush* b = scene->createSceneObject<GenericBrush>(parent);
            if ( vertices.count() < 1 )
                return b;

            b->appendBrushVertices(vertices.toVector());

            for ( int i = 0; i < windings.count(); i++ )
            {
                TexturedWinding* windingFace = windings.at(i);
                if ( windingFace->vertexCount() < 1 )
                    continue;

                GenericBrushFace* face = b->createAndObtainBrushFace();
                face->texturePlane()->setMaterialId(windingFace->materialId());
                face->appendIndices(windingFace->vertexIndices().toVector());
            }

            return b;
        }

        GenericBrush* createBrushFromMinMaxVectors(SceneObject *parent, const QVector3D &min, const QVector3D &max)
        {
            Q_ASSERT_X(parent, Q_FUNC_INFO, "Parent object must be provided!");

            Scene* scene = parent->parentScene();
            Q_ASSERT_X(scene, Q_FUNC_INFO, "Scene must be valid!");

            GenericBrush* b = scene->createSceneObject<GenericBrush>(parent);

            for ( int i = 0; i < 8; i++ )
            {
                const QVector3D& vecX = (i%2 == 0)
                        ? min
                        : max;

                const QVector3D& vecY = ((i/2)%2 == 0)
                        ? min
                        : max;

                const QVector3D& vecZ = ((i/4)%2 == 0)
                        ? min
                        : max;

                b->appendBrushVertex(QVector3D(vecX.x(), vecY.y(), vecZ.z()));
            }

            for ( int i = 0; i < 6; i++ )
            {
                GenericBrushFace* f = b->createAndObtainBrushFace();

                switch (i)
                {
                    case 0: // Normal X+
                    {
                        f->appendIndex(1);
                        f->appendIndex(3);
                        f->appendIndex(7);
                        f->appendIndex(5);
                        break;
                    }

                    case 1: // Normal X-
                    {
                        f->appendIndex(0);
                        f->appendIndex(4);
                        f->appendIndex(6);
                        f->appendIndex(2);
                        break;
                    }

                    case 2: // Normal Y+
                    {
                        f->appendIndex(3);
                        f->appendIndex(2);
                        f->appendIndex(6);
                        f->appendIndex(7);
                        break;
                    }

                    case 3: // Normal Y-
                    {
                        f->appendIndex(0);
                        f->appendIndex(1);
                        f->appendIndex(5);
                        f->appendIndex(4);
                        break;
                    }

                    case 4: // Normal Z+
                    {
                        f->appendIndex(4);
                        f->appendIndex(5);
                        f->appendIndex(7);
                        f->appendIndex(6);
                        break;
                    }

                    case 5: // Normal Z-
                    {
                        f->appendIndex(0);
                        f->appendIndex(2);
                        f->appendIndex(3);
                        f->appendIndex(1);
                        break;
                    }

                    default:
                        break;
                }

                f->texturePlane()->setMaterialId(0);
            }

            return b;
        }
    }
}
