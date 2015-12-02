#ifndef CALLIPERMATH_H
#define CALLIPERMATH_H

#include <QVector3D>
#include <QMatrix4x4>
#include "ceulerangle.h"

namespace Math
{
    QMatrix4x4 matrixTranslate(const QVector3D &translation);
    QMatrix4x4 matrixRotateX(float radians);
    QMatrix4x4 matrixRotateY(float radians);
    QMatrix4x4 matrixRotateZ(float radians);
    QMatrix4x4 matrixScale(const QVector3D &scale);
    QMatrix4x4 matrixScaleUniform(float scale);
    QMatrix4x4 matrixOrientation(const CEulerAngle &angles);

    QVector3D angleToVector(const CEulerAngle &angle, QVector3D* up = NULL);
    CEulerAngle vectorToAngle(const QVector3D &vec);
}

#endif // CALLIPERMATH_H

