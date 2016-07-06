#include "geometryutil.h"
#include "fuzzyvertexmap.h"

namespace GeometryUtil
{
    QList<QVector3D> windingsToVertices(QList<Winding3D*> &windings)
    {
        typedef QLinkedList<WindingVertex> WindingVertexList;
        FuzzyVertexMap vertexMap;

        for ( int i = 0; i < windings.count(); i++ )
        {
            Winding3D* winding = windings[i];
            for ( WindingVertexList::iterator it = winding->verticesBegin(); it != winding->verticesEnd(); ++it )
            {
                int index = vertexMap.mapToIndex(it->position());
                it->setIndex(index);
            }
        }

        return vertexMap.vertexList();
    }

    void clipWindingsWithEachOther(QList<Winding3D *> windings)
    {
        QList<Plane3D> planes;
        for ( int i = 0; i < windings.count(); i++ )
        {
            planes.append(windings.at(i)->plane());
        }

        for ( int i = 0; i < windings.count(); i++ )
        {
            // Cache our plane so we can set it to null in the list.
            Plane3D tempPlane = planes.at(i);
            planes[i] = Plane3D();

            // Clip the winding by the remaining planes.
            windings[i]->clip(planes);

            // Reset the plane.
            planes[i] = tempPlane;
        }
    }
}
