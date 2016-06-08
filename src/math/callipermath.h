#ifndef MATH_H
#define MATH_H

#include <QMatrix4x4>
#include <QVector3D>
#include <QVector2D>
#include "eulerangle.h"
#include <QRectF>

namespace Math
{
    enum AxisIdentifier
    {
        NoAxis = -1,

        AxisX = 0,
        AxisY = 1,
        AxisZ = 2,

        AxisXY = 3,
        AxisYZ = 4,
        AxisXZ = 5,

        AxisXYZ = 6,
    };

    enum AxisFlag
    {
        NoAxisFlag = 0x0,

        AxisFlagX = 0x1,
        AxisFlagY = 0x2,
        AxisFlagZ = 0x4,

        AxisFlagXY = AxisFlagX | AxisFlagY,
        AxisFlagXZ = AxisFlagX | AxisFlagZ,
        AxisFlagYZ = AxisFlagY | AxisFlagZ,

        AxisFlagXYZ = AxisFlagX | AxisFlagY | AxisFlagZ
    };

    namespace StaticMatrix
    {
        const QMatrix4x4& ROT_X_90();
        const QMatrix4x4& ROT_X_180();
        const QMatrix4x4& ROT_X_270();

        const QMatrix4x4& ROT_Y_90();
        const QMatrix4x4& ROT_Y_180();
        const QMatrix4x4& ROT_Y_270();

        const QMatrix4x4& ROT_Z_90();
        const QMatrix4x4& ROT_Z_180();
        const QMatrix4x4& ROT_Z_270();

        const QMatrix4x4& HAMMER_TO_OPENGL();
        const QMatrix4x4& OPENGL_TO_HAMMER();
    }

    QMatrix4x4 matrixTranslate(const QVector3D &translation);
    QMatrix4x4 matrixRotateX(float radians);
    QMatrix4x4 matrixRotateY(float radians);
    QMatrix4x4 matrixRotateZ(float radians);
    QMatrix4x4 matrixScale(const QVector3D &scale);
    QMatrix4x4 matrixScaleUniform(float scale);
    QMatrix4x4 matrixOrientation(const EulerAngle &angle);

    // Window: pixels, (0,0) is top left.
    // Device: (-1,-1) LL to (1,1) UR, (0,0) is centre.
    QMatrix4x4 windowToDevice(int width, int height);
    QMatrix4x4 windowToDevice(const QSize &size);

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
    void perspectivePlane(float fov, float aspectRatio, float distance, QVector3D &min, QVector3D &max);

    bool fuzzyVectorEquals(const QVector2D &v1, const QVector2D &v2);
    bool fuzzyVectorEquals(const QVector3D &v1, const QVector3D &v2);
    bool fuzzyVectorEquals(const QVector4D &v1, const QVector4D &v2);

    bool fuzzyVectorIsNull(const QVector2D &vec);
    bool fuzzyVectorIsNull(const QVector3D &vec);
    bool fuzzyVectorIsNull(const QVector4D &vec);

    QVector3D cardinalAxis(AxisIdentifier axis, bool negated = false);
    void clampToNearestMultiple(QVector3D &vec, qint64 multiple, AxisFlag axisFlags);
}

#endif // MATH_H
