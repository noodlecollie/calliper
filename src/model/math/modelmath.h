#ifndef MODELMATH_H
#define MODELMATH_H

#include "model_global.h"
#include <QMatrix4x4>
#include "eulerangle.h"
#include "winding3d.h"

namespace NS_MODEL
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

        QList<QVector3D> windingsToVertices(QList<Winding3D*> &windings);
        void clipWindingsWithEachOther(QList<Winding3D*> windings);
    }
}

#endif // MODELMATH_H
