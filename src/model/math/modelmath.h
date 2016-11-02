#ifndef MODELMATH_H
#define MODELMATH_H

#include "model_global.h"
#include <QMatrix4x4>
#include "eulerangle.h"
#include "winding3d.h"
#include "texturedwinding.h"
#include <QLinkedList>
#include "fuzzyvertexmap.h"

namespace Model
{
    namespace ModelMath
    {
        QMatrix4x4 matrixOrientation(const EulerAngle &angle);

        // These ignore roll.
        QVector3D angleToVectorSimple(const EulerAngle &angle);
        EulerAngle vectorToAngleSimple(const QVector3D &vec);

        void angleToVectors(const EulerAngle &angle, QVector3D &fwd, QVector3D &right, QVector3D &up);

        // Assumes vectors are normalised and perpendicular.
        EulerAngle vectorsToAngle(const QVector3D &fwd, const QVector3D &up);

        void clipWindingsWithEachOther(Winding3D** windings, int count);

        template<typename T>
        QList<QVector3D> windingsToVertices(const QList<T*>& windings)
        {
            typedef QLinkedList<WindingVertex> WindingVertexList;
            FuzzyVertexMap vertexMap;

            for ( int i = 0; i < windings.count(); i++ )
            {
                Winding3D* winding = windings.at(i);
                for ( WindingVertexList::iterator it = winding->verticesBegin(); it != winding->verticesEnd(); ++it )
                {
                    int index = vertexMap.mapToIndex(it->position());
                    it->setIndex(index);
                }
            }

            return vertexMap.vertexList();
        }

        template<typename T>
        void clipWindingsWithEachOther(const QList<T*>& windings)
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
                windings.at(i)->clip(planes);

                // Reset the plane.
                planes[i] = tempPlane;
            }
        }
    }
}

#endif // MODELMATH_H
