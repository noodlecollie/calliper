#include "callipermath.h"
#include <QtMath>

#define MATARGS_ROT_X(_s, _c) 1,0,0,0, 0,(_c),-(_s),0, 0,(_s),(_c),0, 0,0,0,1
#define MATARGS_ROT_Y(_s, _c) (_c),0,(_s),0, 0,1,0,0, -(_s),0,(_c),0, 0,0,0,1
#define MATARGS_ROT_Z(_s, _c) (_c),-(_s),0,0, (_s),(_c),0,0, 0,0,1,0, 0,0,0,1

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

uint qHash(const QVector3D &vec)
{
    return qHash(QString("%0%1%2").arg(vec.x()).arg(vec.y()).arg(vec.z()));
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

    void angleToVectors(const EulerAngle &angle, QVector3D &fwd, QVector3D &right, QVector3D &up)
    {
        // We already know how to compute the forward vector.
        fwd = angleToVectorSimple(angle);

        // If the forward vector is at a singularity, we return the other vectors as if we
        // had begun by looking down the X axis.
        if ( qFuzzyIsNull(fwd.x()) && qFuzzyIsNull(fwd.y()) )
        {
            float sr = qSin(qDegreesToRadians(angle.roll()));
            float cr = qCos(qDegreesToRadians(angle.roll()));

            if ( fwd.z() < 0.0f )
            {
                up = QVector3D(cr,-sr,0);
            }
            else
            {
                up = QVector3D(-cr,-sr,0);
            }

            right = QVector3D::crossProduct(fwd, up);
            return;
        }

        // We're not pointing down a singularity.
        // Project the forward vector onto the XY plane so we can find an initial right vector.
        // This is just a 90 degree clockwise rotation of the projected vector.
        QVector3D initialRight(fwd.y(), -fwd.x(), 0);
        initialRight.normalize();

        // Now rotate the right vector by a quarternion constructed from the roll.
        // The angle is in degrees, not radians.
        QQuaternion rollRot = QQuaternion::fromAxisAndAngle(fwd, angle.roll());

        // Rotate the right vector.
        right = rollRot.rotatedVector(initialRight);

        // Generate the up vector from these two.
        up = QVector3D::crossProduct(right, fwd);
    }

    EulerAngle vectorsToAngle(const QVector3D &fwd, const QVector3D &up)
    {
        if ( fwd.isNull() || up.isNull() )
        {
            return EulerAngle();
        }

        // Get the and before any roll is applied.
        EulerAngle angle = vectorToAngleSimple(fwd);

        // If we're on a singularity, determine the roll.
        if ( qFuzzyIsNull(fwd.x()) && qFuzzyIsNull(fwd.y()) )
        {
            // This gives us the angle on the wrong side,
            // and it can also be negative.
            float ang = qRadiansToDegrees(qAtan2(up.y(), fwd.z() < 0 ? up.x() : -up.x()));

            // Make the rotation be in the correct direction.
            ang *= -1;

            // Also make sure it's positive.
            while ( ang < 0.0f )
                ang += 360.0f;

            angle.setRoll(ang);
            return angle;
        }

        // Get a right vector that corresponds to the real angles.
        QVector3D right = QVector3D::crossProduct(fwd, up);
        right.normalize();

        // Project the forward vector onto the XY plane.
        QVector3D fwdXY(fwd.x(), fwd.y(), 0);
        fwdXY.normalize();

        // Also get a right basis vector in the XY plane.
        QVector3D rightBasis = QVector3D::crossProduct(fwdXY, QVector3D(0,0,1));

        // Get the dot product between the two forward vectors.
        float fwdDot = QVector3D::dotProduct(fwdXY, fwd);

        // Calculate the angle between these two vectors.
        // This is the pitch.
        float fwdAngDelta = qAcos(fwdDot);

        // As far as we're concerned, the pitch is only between -90 and 90 degrees
        // (or 0-90 and 270-0).
        // If the real forward vector's Z component is negative, the pitch is in
        // the first quadrant. If it's positive, the pitch is in the fourth quadrant.
        // Determine the correct angle from this.
        if ( fwd.z() > 0 )
        {
            fwdAngDelta = 360.0f - fwdAngDelta;
        }

        // Calculate an up basis vector from the pitch angle.
        // This is done by rotating a vertical vector around the right vector in the XY plane.
        QQuaternion createUpBasis = QQuaternion::fromAxisAndAngle(rightBasis, fwdAngDelta);
        QVector3D upBasis = createUpBasis.rotatedVector(QVector3D(0,0,1));

        // We can use the right and up basis vectors to project the current right vector.
        // This will give new X and Y co-ords in the plane, so we can calculate the angle.

        float xc = QVector3D::dotProduct(right, rightBasis);
        float yc = QVector3D::dotProduct(right, upBasis);
        float ang = qRadiansToDegrees(qAtan2(yc, xc));

        // Make the rotation be in the correct direction.
        ang *= -1;

        // Also make sure it's positive.
        while ( ang < 0.0f )
            ang += 360.0f;

        angle.setRoll(ang);
        return angle;
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
