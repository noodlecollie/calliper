#ifndef MATH_H
#define MATH_H

#include "calliperutil_global.h"
#include <QMatrix4x4>
#include <QVector3D>
#include <QVector2D>
#include <QRectF>
#include <functional>

namespace CalliperUtil
{
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

        // These are based off Hammer for now.
        namespace CoordinateSystem
        {
            CALLIPERUTILSHARED_EXPORT int max();
            CALLIPERUTILSHARED_EXPORT int min();
            CALLIPERUTILSHARED_EXPORT int extent();         // Distance from min (-) to max (+)
            CALLIPERUTILSHARED_EXPORT double diagonal();    // Distance from opposite corners of a square whose sides are of length extent()
        }

        namespace StaticMatrix
        {
            CALLIPERUTILSHARED_EXPORT const QMatrix4x4& ROT_X_90();
            CALLIPERUTILSHARED_EXPORT const QMatrix4x4& ROT_X_180();
            CALLIPERUTILSHARED_EXPORT const QMatrix4x4& ROT_X_270();

            CALLIPERUTILSHARED_EXPORT const QMatrix4x4& ROT_Y_90();
            CALLIPERUTILSHARED_EXPORT const QMatrix4x4& ROT_Y_180();
            CALLIPERUTILSHARED_EXPORT const QMatrix4x4& ROT_Y_270();

            CALLIPERUTILSHARED_EXPORT const QMatrix4x4& ROT_Z_90();
            CALLIPERUTILSHARED_EXPORT const QMatrix4x4& ROT_Z_180();
            CALLIPERUTILSHARED_EXPORT const QMatrix4x4& ROT_Z_270();

            CALLIPERUTILSHARED_EXPORT const QMatrix4x4& HAMMER_TO_OPENGL();
            CALLIPERUTILSHARED_EXPORT const QMatrix4x4& OPENGL_TO_HAMMER();
        }

        CALLIPERUTILSHARED_EXPORT QMatrix4x4 matrixTranslate(const QVector3D &translation);
        CALLIPERUTILSHARED_EXPORT QMatrix4x4 matrixRotateX(float radians);
        CALLIPERUTILSHARED_EXPORT QMatrix4x4 matrixRotateY(float radians);
        CALLIPERUTILSHARED_EXPORT QMatrix4x4 matrixRotateZ(float radians);
        CALLIPERUTILSHARED_EXPORT QMatrix4x4 matrixScale(const QVector3D &scale);
        CALLIPERUTILSHARED_EXPORT QMatrix4x4 matrixScaleUniform(float scale);

        // Window: pixels, (0,0) is top left.
        // Device: (-1,-1) LL to (1,1) UR, (0,0) is centre.
        CALLIPERUTILSHARED_EXPORT QMatrix4x4 windowToDevice(int width, int height);
        CALLIPERUTILSHARED_EXPORT QMatrix4x4 windowToDevice(const QSize &size);

        // Positive values of x only!
        inline bool isPowerOfTwo (int x)
        {
          return ((x != 0) && !(x & (x - 1)));
        }

        CALLIPERUTILSHARED_EXPORT qint64 previousMultiple(float value, qint64 multiple);
        CALLIPERUTILSHARED_EXPORT qint64 nextMultiple(float value, qint64 multiple);
        CALLIPERUTILSHARED_EXPORT qint64 nearestMultiple(float value, qint64 multiple);
        CALLIPERUTILSHARED_EXPORT void clampToNearestMultiple(QVector3D &vec, qint64 multiple);

        CALLIPERUTILSHARED_EXPORT qint64 alignedValue(qint64 value, qint64 alignment);

        CALLIPERUTILSHARED_EXPORT void perspectivePlane(float fov, float aspectRatio, float distance, QVector3D &min, QVector3D &max);

        CALLIPERUTILSHARED_EXPORT bool fuzzyVectorEquals(const QVector2D &v1, const QVector2D &v2);
        CALLIPERUTILSHARED_EXPORT bool fuzzyVectorEquals(const QVector3D &v1, const QVector3D &v2);
        CALLIPERUTILSHARED_EXPORT bool fuzzyVectorEquals(const QVector4D &v1, const QVector4D &v2);

        CALLIPERUTILSHARED_EXPORT bool fuzzyVectorIsNull(const QVector2D &vec);
        CALLIPERUTILSHARED_EXPORT bool fuzzyVectorIsNull(const QVector3D &vec);
        CALLIPERUTILSHARED_EXPORT bool fuzzyVectorIsNull(const QVector4D &vec);

        CALLIPERUTILSHARED_EXPORT QVector3D cardinalAxis(AxisIdentifier axis, bool negated = false);
        CALLIPERUTILSHARED_EXPORT void clampToNearestMultiple(QVector3D &vec, qint64 multiple, AxisFlag axisFlags);

        // Assumes normal is of unit length.
        // The output vector is not normalised.
        CALLIPERUTILSHARED_EXPORT QVector3D vectorPerpendicularTo(const QVector3D &orig, const QVector3D &normal);

        // If close enough to a unit grid point, sets the vector to the point.
        CALLIPERUTILSHARED_EXPORT void fuzzyAdjustToUnitGrid(QVector3D &vec);

        CALLIPERUTILSHARED_EXPORT float distanceFromPointToPlane(const QVector3D &point, const QVector3D &planeNormal, const QVector3D pointOnPlane);

        inline static QVector3D transformVectorPosition(const QVector3D& vec, const QMatrix4x4& mat)
        {
            return (QVector4D(vec, 1) * mat).toVector3D();
        }

        inline static QVector3D transformVectorDirection(const QVector3D vec, const QMatrix4x4& mat)
        {
            return (QVector4D(vec, 0) * mat).toVector3D();
        }
    }

    CALLIPERUTILSHARED_EXPORT uint qHash(const QVector3D &vec, uint seed);
}

#endif // MATH_H
