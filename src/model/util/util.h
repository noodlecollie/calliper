#ifndef UTIL_H
#define UTIL_H

#include "model_global.h"

namespace NS_MODEL
{
    namespace Util
    {
        // Format (x y z)
        MODELSHARED_EXPORT QVector3D vectorFromVmfCoord(const QString &coord);

        // Format (x y z) (x y z) (x y z)
        MODELSHARED_EXPORT void vectorsFromVmfCoords(const QString &coords, QVector3D &v0, QVector3D &v1, QVector3D &v2);

        // These ignore roll.
        MODELSHARED_EXPORT QVector3D angleToVectorSimple(const EulerAngle &angle);
        MODELSHARED_EXPORT EulerAngle vectorToAngleSimple(const QVector3D &vec);

        MODELSHARED_EXPORT void angleToVectors(const EulerAngle &angle, QVector3D &fwd, QVector3D &right, QVector3D &up);

        // Assumes vectors are normalised and perpendicular.
        MODELSHARED_EXPORT EulerAngle vectorsToAngle(const QVector3D &fwd, const QVector3D &up);

        MODELSHARED_EXPORT void cameraOrientationVectors(const EulerAngle &angle, QVector3D &fwd, QVector3D &right, QVector3D &up);
        MODELSHARED_EXPORT QMatrix4x4 matrixOrientation(const EulerAngle &angle);
    }
}

#endif // UTIL_H
