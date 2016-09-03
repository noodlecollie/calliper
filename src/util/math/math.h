#ifndef MATH_H
#define MATH_H

#include "util_global.h"
#include <QMatrix4x4>
#include <QVector3D>
#include <QVector2D>
#include <QRectF>
#include <functional>

namespace NS_UTIL
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
            UTILSHARED_EXPORT int max();
            UTILSHARED_EXPORT int min();
            UTILSHARED_EXPORT int extent();
            UTILSHARED_EXPORT double diagonal();
        }

        namespace StaticMatrix
        {
            UTILSHARED_EXPORT const QMatrix4x4& ROT_X_90();
            UTILSHARED_EXPORT const QMatrix4x4& ROT_X_180();
            UTILSHARED_EXPORT const QMatrix4x4& ROT_X_270();

            UTILSHARED_EXPORT const QMatrix4x4& ROT_Y_90();
            UTILSHARED_EXPORT const QMatrix4x4& ROT_Y_180();
            UTILSHARED_EXPORT const QMatrix4x4& ROT_Y_270();

            UTILSHARED_EXPORT const QMatrix4x4& ROT_Z_90();
            UTILSHARED_EXPORT const QMatrix4x4& ROT_Z_180();
            UTILSHARED_EXPORT const QMatrix4x4& ROT_Z_270();

            UTILSHARED_EXPORT const QMatrix4x4& HAMMER_TO_OPENGL();
            UTILSHARED_EXPORT const QMatrix4x4& OPENGL_TO_HAMMER();
        }

        UTILSHARED_EXPORT QMatrix4x4 matrixTranslate(const QVector3D &translation);
        UTILSHARED_EXPORT QMatrix4x4 matrixRotateX(float radians);
        UTILSHARED_EXPORT QMatrix4x4 matrixRotateY(float radians);
        UTILSHARED_EXPORT QMatrix4x4 matrixRotateZ(float radians);
        UTILSHARED_EXPORT QMatrix4x4 matrixScale(const QVector3D &scale);
        UTILSHARED_EXPORT QMatrix4x4 matrixScaleUniform(float scale);

        // Window: pixels, (0,0) is top left.
        // Device: (-1,-1) LL to (1,1) UR, (0,0) is centre.
        UTILSHARED_EXPORT QMatrix4x4 windowToDevice(int width, int height);
        UTILSHARED_EXPORT QMatrix4x4 windowToDevice(const QSize &size);

        // Positive values of x only!
        inline bool isPowerOfTwo (int x)
        {
          return ((x != 0) && !(x & (x - 1)));
        }

        UTILSHARED_EXPORT qint64 previousMultiple(float value, qint64 multiple);
        UTILSHARED_EXPORT qint64 nextMultiple(float value, qint64 multiple);
        UTILSHARED_EXPORT qint64 nearestMultiple(float value, qint64 multiple);
        UTILSHARED_EXPORT void clampToNearestMultiple(QVector3D &vec, qint64 multiple);

        UTILSHARED_EXPORT void perspectivePlane(float fov, float aspectRatio, float distance, QVector3D &min, QVector3D &max);

        UTILSHARED_EXPORT bool fuzzyVectorEquals(const QVector2D &v1, const QVector2D &v2);
        UTILSHARED_EXPORT bool fuzzyVectorEquals(const QVector3D &v1, const QVector3D &v2);
        UTILSHARED_EXPORT bool fuzzyVectorEquals(const QVector4D &v1, const QVector4D &v2);

        UTILSHARED_EXPORT bool fuzzyVectorIsNull(const QVector2D &vec);
        UTILSHARED_EXPORT bool fuzzyVectorIsNull(const QVector3D &vec);
        UTILSHARED_EXPORT bool fuzzyVectorIsNull(const QVector4D &vec);

        UTILSHARED_EXPORT QVector3D cardinalAxis(AxisIdentifier axis, bool negated = false);
        UTILSHARED_EXPORT void clampToNearestMultiple(QVector3D &vec, qint64 multiple, AxisFlag axisFlags);

        // Assumes normal is of unit length.
        // The output vector is not normalised.
        UTILSHARED_EXPORT QVector3D vectorPerpendicularTo(const QVector3D &orig, const QVector3D &normal);

        // If close enough to a unit grid point, sets the vector to the point.
        UTILSHARED_EXPORT void fuzzyAdjustToUnitGrid(QVector3D &vec);

        UTILSHARED_EXPORT float distanceFromPointToPlane(const QVector3D &point, const QVector3D &planeNormal, const QVector3D pointOnPlane);

        template<typename T>
        void updateWithInitCheck(const T &value, T &toUpdate, bool &hasBeenUpdated,
                                 std::function<bool (const T& val, const T& current)> updateFunc)
        {
            if ( !hasBeenUpdated || updateFunc(value, toUpdate) )
            {
                toUpdate = value;
                hasBeenUpdated = true;
            }
        }

        template<typename T>
        void updateIfLessThan(const T &value, T &toUpdate, bool &hasBeenUpdated)
        {
            return updateWithInitCheck<T>(value, toUpdate, hasBeenUpdated,
                                       [] (const T &val, const T &current)
                                          {return val < current;});
        }

        template<typename T>
        void updateIfGreaterThan(const T &value, T &toUpdate, bool &hasBeenUpdated)
        {
            return updateWithInitCheck<T>(value, toUpdate, hasBeenUpdated,
                                       [] (const T &val, const T &current)
                                          {return val > current;});
        }
    }

    UTILSHARED_EXPORT uint qHash(const QVector3D &vec, uint seed);
}

#endif // MATH_H
