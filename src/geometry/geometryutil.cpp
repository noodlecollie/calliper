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
                it->setIndex(vertexMap.mapToIndex(it->position()));
            }
        }

        return vertexMap.vertexList();
    }
}
