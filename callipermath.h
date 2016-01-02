#ifndef MATH_H
#define MATH_H

#include <QMatrix4x4>
#include <QVector3D>
#include "eulerangle.h"

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
}

#endif // MATH_H
