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

    QVector3D angleToVectorSimple(const EulerAngle &angle)
    {
        // X and Y are dependent on the pitch and yaw; Z is dependent on the pitch only.
        float radYaw = qDegreesToRadians(angle.yaw());
        float radPitch = qDegreesToRadians(angle.pitch());
        float sinPitch = qSin(radPitch);
        float sinYaw = qSin(radYaw);
        float cosPitch = qCos(radPitch);
        float cosYaw = qCos(radYaw);

        return QVector3D(cosPitch * cosYaw, cosPitch * sinYaw, -sinPitch);
    }

    EulerAngle vectorToAngleSimple(const QVector3D &vec)
    {
        // http://www.gamedev.net/topic/399701-convert-vector-to-euler-cardan-angles/#entry3651854

        // If the vector is null, return a zeroed angle.
        if ( vec.isNull() )
        {
            return EulerAngle();
        }

        // If x and y are null, just set the pitch.
        if ( qFuzzyIsNull(vec.x()) && qFuzzyIsNull(vec.y()) )
        {
            return EulerAngle(vec.z() > 0.0f ? 270.0f : 90.0f, 0.0f, 0.0f);
        }

        float temp, yaw, pitch;

        // Yaw depends on the x and y co-ordinates.
        yaw = qRadiansToDegrees(qAtan2(vec.y(), vec.x()));
        if ( yaw < 0.0f ) yaw += 360.0f;

        // Pitch is found by finding the angle between the xy projection of the vector
        // and the negative Z axis.
        temp = qSqrt(vec.x()*vec.x() + vec.y()*vec.y());        // Length of projection onto xy plane
        pitch = qRadiansToDegrees(qAtan(-vec.z() / temp));      // Angle between this and -z.
        if ( pitch < 0.0f ) pitch += 360;

        return EulerAngle(pitch, yaw, 0.0f);
    }

    QMatrix4x4 matrixOrientation(const EulerAngle &angle)
    {
        return matrixRotateZ(qDegreesToRadians(angle.yaw()))
                * matrixRotateY(qDegreesToRadians(angle.pitch()))
                * matrixRotateX(qDegreesToRadians(angle.roll()));
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

    void cameraOrientationVectors(const EulerAngle &angle, QVector3D &fwd, QVector3D &right, QVector3D &up)
    {
        // Convert the angle to a vector.
        fwd = angleToVectorSimple(angle);

        // TODO: Correctly handle roll.
        // Right now we just set the right vector to be the projection onto the XY plane.
        right = (matrixRotateZ(qDegreesToRadians(-90.0f)) * QVector4D(fwd.x(), fwd.y(), 0, 0)).toVector3D();
        if ( right.isNull() )
        {
            up = QVector3D();
            return;
        }

        right.normalize();

        // The up vector is a cross product between the other two.
        up = QVector3D::crossProduct(right, fwd);
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
}
