#include "callipermath.h"
#include <QtMath>

static const QMatrix4x4 HAMMER_TO_OPENGL(
        1, 0, 0, 0,
        0, 0, 1, 0,
        0,-1, 0, 0,
        0, 0, 0, 1
        );

static const QMatrix4x4 OPENGL_TO_HAMMER(
        1, 0, 0, 0,
        0, 0,-1, 0,
        0, 1, 0, 0,
        0, 0, 0, 1
        );

QMatrix4x4 _matRotX(float c, float s)
{
    return QMatrix4x4(1, 0, 0, 0,
                      0, c, -s, 0,
                      0, s, c, 0,
                      0, 0, 0, 1);
}

QMatrix4x4 _matRotY(float c, float s)
{
    return QMatrix4x4(c, 0, s, 0,
                      0, 1, 0, 0,
                      -s, 0, c, 0,
                      0, 0, 0, 1);
}

QMatrix4x4 _matRotZ(float c, float s)
{
    return QMatrix4x4(c, -s, 0, 0,
                      s, c, 0, 0,
                      0, 0, 1, 0,
                      0, 0, 0, 1);
}

namespace Math
{
    QMatrix4x4 matrixTranslate(const QVector3D &translation)
    {
        return QMatrix4x4(1, 0, 0, translation.x(),
                          0, 1, 0, translation.y(),
                          0, 0, 1, translation.z(),
                          0, 0, 0, 1);
    }

    QMatrix4x4 matrixRotateX(float radians)
    {
        return _matRotX(qCos(radians), qSin(radians));
    }

    QMatrix4x4 matrixRotateY(float radians)
    {
        return _matRotY(qCos(radians), qSin(radians));
    }

    QMatrix4x4 matrixRotateZ(float radians)
    {
        return _matRotZ(qCos(radians), qSin(radians));
    }

    QMatrix4x4 matrixScale(const QVector3D &scale)
    {
        return QMatrix4x4(scale.x(), 0, 0, 0,
                          0, scale.y(), 0, 0,
                          0, 0, scale.z(), 0,
                          0, 0, 0, 1);
    }

    QMatrix4x4 matrixScaleUniform(float scale)
    {
        return matrixScale(QVector3D(scale, scale, scale));
    }

    const QMatrix4x4& hammerToOpenGL()
    {
        return HAMMER_TO_OPENGL;
    }

    const QMatrix4x4& openGLToHammer()
    {
        return OPENGL_TO_HAMMER;
    }
}

