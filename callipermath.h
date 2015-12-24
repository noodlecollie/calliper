#ifndef MATH_H
#define MATH_H

#include <QMatrix4x4>
#include <QVector3D>

namespace Math
{
    QMatrix4x4 matrixTranslate(const QVector3D &translation);
    QMatrix4x4 matrixRotateX(float radians);
    QMatrix4x4 matrixRotateY(float radians);
    QMatrix4x4 matrixRotateZ(float radians);
    QMatrix4x4 matrixScale(const QVector3D &scale);
    QMatrix4x4 matrixScaleUniform(float scale);

    const QMatrix4x4& hammerToOpenGL();
    const QMatrix4x4& openGLToHammer();
}

#endif // MATH_H
