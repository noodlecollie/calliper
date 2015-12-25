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

    // These ignore roll.
    QVector3D angleToVectorSimple(const EulerAngle &angle);
    EulerAngle vectorToAngleSimple(const QVector3D &vec);
}

#endif // MATH_H
