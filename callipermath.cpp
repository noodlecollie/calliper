#include "callipermath.h"
#include <QtMath>

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

    CEulerAngle vectorToAngle(const QVector3D &vec)
    {
        // http://www.gamedev.net/topic/399701-convert-vector-to-euler-cardan-angles/#entry3651854

        // If the vector is null, return a zeroed angle.
        if ( vec.isNull() )
        {
            return CEulerAngle();
        }

        // If x and y are null, just set the pitch.
        if ( qFuzzyIsNull(vec.x()) && qFuzzyIsNull(vec.y()) )
        {
            return CEulerAngle(vec.z() > 0.0f ? 270.0f : 90.0f, 0.0f, 0.0f);
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

        return CEulerAngle(pitch, yaw, 0.0f);
    }

    QVector3D angleToVector(const CEulerAngle &angle, QVector3D* up)
    {
        // TODO: Take roll into account!

        // X and Y are dependent on the pitch and yaw; Z is dependent on the pitch only.
        float radYaw = qDegreesToRadians(angle.yaw());
        float radPitch = qDegreesToRadians(angle.pitch());
        float sinPitch = qSin(radPitch);
        float sinYaw = qSin(radYaw);
        float cosPitch = qCos(radPitch);
        float cosYaw = qCos(radYaw);

        // TODO: Test this?
        if ( up )
        {
            // If pitch is within -90 to 90 degrees, the up vector will always be >= 0 in Z,
            // ie. the Z component is related to cos of pitch.
            up->setZ(cosPitch);

            // The X and Y components depend on pitch and yaw: where abs(pitch) is smaller
            // between -90 and 90, X and Y vary more. Therefore they vary with the sin of pitch.
            up->setX(sinPitch * cosYaw);
            up->setY(sinPitch * sinYaw);
        }

        return QVector3D(cosPitch * cosYaw, cosPitch * sinYaw, -sinPitch);
    }

    QMatrix4x4 matrixOrientation(const CEulerAngle &angles)
    {
        // TODO: Test me!!
        return matrixRotateZ(angles.yaw()) * matrixRotateY(angles.pitch()) * matrixRotateX(angles.roll());
    }
}
