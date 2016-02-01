#ifndef MATH_H
#define MATH_H

#include <QMatrix4x4>
#include <QVector3D>
#include "eulerangle.h"
#include <QSizeF>

namespace Math
{
    QMatrix4x4 matrixTranslate(const QVector3D &translation);
    QMatrix4x4 matrixRotateX(float radians);
    QMatrix4x4 matrixRotateY(float radians);
    QMatrix4x4 matrixRotateZ(float radians);
    QMatrix4x4 matrixScale(const QVector3D &scale);
    QMatrix4x4 matrixScaleUniform(float scale);
    QMatrix4x4 matrixOrientation(const EulerAngle &angle);

    const QMatrix4x4& hammerToOpenGL();
    const QMatrix4x4& openGLToHammer();

    // Window: pixels, (0,0) is top left.
    // Device: (-1,-1) LL to (1,1) UR, (0,0) is centre.
    QMatrix4x4 windowToDevice(int width, int height);

    // These ignore roll.
    QVector3D angleToVectorSimple(const EulerAngle &angle);
    EulerAngle vectorToAngleSimple(const QVector3D &vec);

    // Positive values of x only!
    inline bool isPowerOfTwo (int x)
    {
      return ((x != 0) && !(x & (x - 1)));
    }

    qint64 previousMultiple(float value, qint64 multiple);
    qint64 nextMultiple(float value, qint64 multiple);
    qint64 nearestMultiple(float value, qint64 multiple);
    void clampToNearestMultiple(QVector3D &vec, qint64 multiple);

    void cameraOrientationVectors(const EulerAngle &angle, QVector3D &fwd, QVector3D &right, QVector3D &up);

    // Returns the size of a camera plane in camera space. The centre of the plane
    // is the centroid of the size rectangle.
    QSizeF cameraPlaneSize(float fov, float aspectRatio, float distance);
}

#endif // MATH_H
