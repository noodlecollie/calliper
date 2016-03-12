#include "brushfactory.h"
#include "mapscene.h"
#include "callipermath.h"
#include "brushface.h"
#include "textureplane.h"

namespace BrushFactory
{
    Brush* fromBoundingBox(const BoundingBox &bbox, SceneObject *parent, const QString &texture)
    {
        // Get the vertices from the bounding box.
        // X alternates most frequently, then Y and then Z.
        QList<QVector3D> verts = bbox.corners();

        // Add these to the brush.
        Brush* b = new Brush(parent);
        b->appendVertices(verts);

        // Create the faces.
        // Because of the way the vertices are specified, the indexing order is modified.
        for ( int i = 0; i < 6; i++ )
        {
            BrushFace* f = new BrushFace(b);
            f->setObjectName(QString("face%0").arg(i));
            f->texturePlane()->setTexturePath(texture);

            switch (i)
            {
                case 0: // X min
                {
                    f->appendVertex(0);
                    f->appendVertex(4);
                    f->appendVertex(6);
                    f->appendVertex(2);
                    break;
                }

                case 1: // X max
                {
                    f->appendVertex(1);
                    f->appendVertex(3);
                    f->appendVertex(7);
                    f->appendVertex(5);
                    break;
                }

                case 2: // Y min
                {
                    f->appendVertex(0);
                    f->appendVertex(1);
                    f->appendVertex(5);
                    f->appendVertex(4);
                    break;
                }

                case 3: // Y max
                {
                    f->appendVertex(2);
                    f->appendVertex(6);
                    f->appendVertex(7);
                    f->appendVertex(3);
                    break;
                }

                case 4: // Z min
                {
                    f->appendVertex(0);
                    f->appendVertex(2);
                    f->appendVertex(3);
                    f->appendVertex(1);
                    break;
                }

                case 5: // Z max
                {
                    f->appendVertex(4);
                    f->appendVertex(5);
                    f->appendVertex(7);
                    f->appendVertex(6);
                    break;
                }

                default:
                    break;
            }
        }

        return b;
    }
}
