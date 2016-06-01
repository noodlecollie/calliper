#include "brushfactory.h"
#include "mapscene.h"
#include "callipermath.h"
#include "brushface.h"
#include "textureplane.h"

void appendCuboidFace(BrushFace* face, Math::AxisIdentifier axis, bool frontFace)
{
    const int* indices = BoundingBox::cornerVerticesForFace(axis);
    int offset = frontFace ? 0 : 4;
    for ( int i = 0; i < 4; i++ )
    {
        face->appendVertex(indices[offset + i]);
    }
}

namespace BrushFactory
{
    Brush* fromBoundingBox(BaseScene *scene, SceneObject *parent, const BoundingBox &bbox, const QString &texture, const QVector3D origin)
    {
        // Get the vertices from the bounding box.
        // X alternates most frequently, then Y and then Z.
        QVector<QVector3D> verts = bbox.transformed(Math::matrixTranslate(-origin)).corners().toVector();

        // Add these to the brush.
        Brush* b = scene->createSceneObject<Brush>(parent);
        b->setPosition(origin);
        b->appendVertices(verts);

        // Create the faces.
        // Because of the way the vertices are specified, the indexing order is modified.
        for ( int i = 0; i < 6; i++ )
        {
            BrushFace* f = new BrushFace(b);
            f->setObjectName(QString("face%0").arg(i));
            f->texturePlane()->setTexturePath(texture);

            appendCuboidFace(f, (Math::AxisIdentifier)(i%3), i < 3);
        }

        return b;
    }
}
