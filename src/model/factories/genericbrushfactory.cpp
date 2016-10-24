#include "genericbrushfactory.h"
#include "math/modelmath.h"
#include "scene/scene.h"

namespace NS_MODEL
{
    namespace GenericBrushFactory
    {
        GenericBrush* createBrushFromWindingGroup(SceneObject* parent, const QList<TexturedWinding*> windings)
        {
            Q_ASSERT_X(parent, Q_FUNC_INFO, "Parent object must be provided!");

            Scene* scene = parent->parentScene();
            Q_ASSERT_X(scene, Q_FUNC_INFO, "Scene must be valid!");

            // Get a collection of vertices.
            QList<Winding3D*> baseWindings;
            foreach ( TexturedWinding* p, windings )
            {
                baseWindings.append(p);
            }

            QList<QVector3D> vertices = ModelMath::windingsToVertices(baseWindings);

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
                face->texturePlane()->setTextureId(windingFace->textureId());
            }

            return b;
        }
    }
}
