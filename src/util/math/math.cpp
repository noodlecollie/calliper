#include "math.h"
#include <QtMath>
#include <QByteArray>
#include <QDataStream>

#define MATARGS_ROT_X(_s, _c) 1,0,0,0, 0,(_c),-(_s),0, 0,(_s),(_c),0, 0,0,0,1
#define MATARGS_ROT_Y(_s, _c) (_c),0,(_s),0, 0,1,0,0, -(_s),0,(_c),0, 0,0,0,1
#define MATARGS_ROT_Z(_s, _c) (_c),-(_s),0,0, (_s),(_c),0,0, 0,0,1,0, 0,0,0,1

namespace NS_UTIL
{
    uint qHash(const QVector3D &vec, uint seed)
    {
        QByteArray array;

        {
            QDataStream stream(&array, QIODevice::WriteOnly);
            stream << vec.x() << vec.y() << vec.z();
        }

        return qHash(array, seed);
    }

    static const QMatrix4x4 M_HAMMER_TO_OPENGL(
            1, 0, 0, 0,
            0, 0, 1, 0,
            0,-1, 0, 0,
            0, 0, 0, 1
            );

    static const QMatrix4x4 M_OPENGL_TO_HAMMER(
            1, 0, 0, 0,
            0, 0,-1, 0,
            0, 1, 0, 0,
            0, 0, 0, 1
            );

    static const QMatrix4x4 M_ROT_X_90(MATARGS_ROT_X(1, 0));
    static const QMatrix4x4 M_ROT_X_180(MATARGS_ROT_X(0, -1));
    static const QMatrix4x4 M_ROT_X_270(MATARGS_ROT_X(-1, 0));

    static const QMatrix4x4 M_ROT_Y_90(MATARGS_ROT_Y(1, 0));
    static const QMatrix4x4 M_ROT_Y_180(MATARGS_ROT_Y(0, -1));
    static const QMatrix4x4 M_ROT_Y_270(MATARGS_ROT_Y(-1, 0));

    static const QMatrix4x4 M_ROT_Z_90(MATARGS_ROT_Z(1, 0));
    static const QMatrix4x4 M_ROT_Z_180(MATARGS_ROT_Z(0, -1));
    static const QMatrix4x4 M_ROT_Z_270(MATARGS_ROT_Z(-1, 0));

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
        namespace StaticMatrix
        {
            const QMatrix4x4& ROT_X_90() { return M_ROT_X_90; }
            const QMatrix4x4& ROT_X_180() { return M_ROT_X_180; }
            const QMatrix4x4& ROT_X_270() { return M_ROT_X_270; }

            const QMatrix4x4& ROT_Y_90() { return M_ROT_Y_90; }
            const QMatrix4x4& ROT_Y_180() { return M_ROT_Y_180; }
            const QMatrix4x4& ROT_Y_270() { return M_ROT_Y_270; }

            const QMatrix4x4& ROT_Z_90() { return M_ROT_Z_90; }
            const QMatrix4x4& ROT_Z_180() { return M_ROT_Z_180; }
            const QMatrix4x4& ROT_Z_270() { return M_ROT_Z_270; }

            const QMatrix4x4& HAMMER_TO_OPENGL() { return M_HAMMER_TO_OPENGL; }
            const QMatrix4x4& OPENGL_TO_HAMMER() { return M_OPENGL_TO_HAMMER; }
        }

        namespace CoordinateSystem
        {
            int max()
            {
                return 16384;
            }

            int min()
            {
                return -max();
            }

            int extent()
            {
                return 2*max();
            }

            double diagonal()
            {
                return 1.732050807569 * extent();
            }
        }

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

        QMatrix4x4 windowToDevice(int width, int height)
        {
            float w = width, h = height;
            return QMatrix4x4(2/w, 0, 0, (1-w)/w,
                              0, -2/h, 0, (h-1)/h,
                              0, 0, 1, 0,
                              0, 0, 0, 1);
        }

        QMatrix4x4 windowToDevice(const QSize &size)
        {
            return windowToDevice(size.width(), size.height());
        }

        qint64 previousMultiple(float value, qint64 multiple)
        {
            return qFloor(value/multiple) * multiple;
        }

        qint64 nextMultiple(float value, qint64 multiple)
        {
            return previousMultiple(value, multiple) + multiple;
        }

        qint64 nearestMultiple(float value, qint64 multiple)
        {
            qint64 prev = previousMultiple(value, multiple);
            qint64 next = nextMultiple(value, multiple);

            float dPrev = value - (float)prev;
            float dNext = (float)next - value;
            return dPrev < dNext ? prev : next;
        }

        void clampToNearestMultiple(QVector3D &vec, qint64 multiple)
        {
            for ( int i = 0; i < 3; i++ )
            {
                vec[i] = (float)nearestMultiple(vec[i], multiple);
            }
        }

        void perspectivePlane(float fov, float aspectRatio, float distance, QVector3D &min, QVector3D &max)
        {
            float top = distance * qTan(qDegreesToRadians(fov/2.0f));
            float bottom = -top;
            float right = top * aspectRatio;
            float left = -right;

            min = QVector3D(left, bottom, distance);
            max = QVector3D(right, top, distance);
        }

        bool fuzzyVectorEquals(const QVector2D &v1, const QVector2D &v2)
        {
            return (fuzzyVectorIsNull(v1) && fuzzyVectorIsNull(v2)) ||
                    (qFuzzyCompare(v1.x(), v2.x()) &&
                    qFuzzyCompare(v1.y(), v2.y()));
        }

        bool fuzzyVectorEquals(const QVector3D &v1, const QVector3D &v2)
        {
            return (fuzzyVectorIsNull(v1) && fuzzyVectorIsNull(v2)) ||
                    (qFuzzyCompare(v1.x(), v2.x()) &&
                    qFuzzyCompare(v1.y(), v2.y()) &&
                    qFuzzyCompare(v1.z(), v2.z()));
        }

        bool fuzzyVectorEquals(const QVector4D &v1, const QVector4D &v2)
        {
            return (fuzzyVectorIsNull(v1) && fuzzyVectorIsNull(v2)) ||
                    (qFuzzyCompare(v1.x(), v2.x()) &&
                    qFuzzyCompare(v1.y(), v2.y()) &&
                    qFuzzyCompare(v1.z(), v2.z()) &&
                    qFuzzyCompare(v1.w(), v2.w()));
        }

        bool fuzzyVectorIsNull(const QVector2D &vec)
        {
            return qFuzzyIsNull(vec.x()) &&
                    qFuzzyIsNull(vec.y());
        }

        bool fuzzyVectorIsNull(const QVector3D &vec)
        {
            return qFuzzyIsNull(vec.x()) &&
                    qFuzzyIsNull(vec.y()) &&
                    qFuzzyIsNull(vec.z());
        }

        bool fuzzyVectorIsNull(const QVector4D &vec)
        {
            return qFuzzyIsNull(vec.x()) &&
                    qFuzzyIsNull(vec.y()) &&
                    qFuzzyIsNull(vec.z()) &&
                    qFuzzyIsNull(vec.w());
        }

        QVector3D cardinalAxis(AxisIdentifier axis, bool negated)
        {
            switch (axis)
            {
            case AxisX:
                return QVector3D(negated ? -1 : 1, 0, 0);

            case AxisY:
                return QVector3D(0, negated ? -1 : 1, 0);

            case AxisZ:
                return QVector3D(0, 0, negated ? -1 : 1);

            default:
                Q_ASSERT(false);
                return QVector3D();
            }
        }

        void clampToNearestMultiple(QVector3D &vec, qint64 multiple, AxisFlag axisFlags)
        {
            for ( int i = 0; i < 3; i++ )
            {
                int flag = 1 << i;
                if ( (axisFlags & flag) != flag )
                    continue;

                vec[i] = (float)nearestMultiple(vec[i], multiple);
            }
        }

        QVector3D vectorPerpendicularTo(const QVector3D &orig, const QVector3D &normal)
        {
            return orig - (QVector3D::dotProduct(orig, normal) * normal);
        }

        void fuzzyAdjustToUnitGrid(QVector3D &vec)
        {
            QVector3D unit;
            for ( int i = 0; i < 3; i++ )
            {
                unit[i] = qRound(vec[i]);
                if ( !qFuzzyCompare(vec[i], unit[i]) )
                    return;
            }
            vec = unit;
        }

        float distanceFromPointToPlane(const QVector3D &point, const QVector3D &planeNormal, const QVector3D pointOnPlane)
        {
            // v = vector from point on plane to target point.
            // n = normal
            // distance = v dot n
            return QVector3D::dotProduct(point - pointOnPlane, planeNormal);
        }
    }
}
