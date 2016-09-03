#include "util.h"

namespace NS_MODEL
{
    namespace Util
    {
        // TODO: Error handling.
        QVector3D vectorFromVmfCoord(const QString &coord)
        {
            QStringList list = coord.split(" ");
            list[0] = list[0].right(list[0].length() - 1);
            list[2] = list[2].left(list[2].length() - 1);

            return QVector3D(list.at(0).toFloat(),
                             list.at(1).toFloat(),
                             list.at(2).toFloat());
        }

        // TODO: Error handling.
        void vectorsFromVmfCoords(const QString &coords, QVector3D &v0, QVector3D &v1, QVector3D &v2)
        {
            int sep1 = coords.indexOf(')') + 1;
            int sep2 = coords.indexOf(')', sep1) + 1;

            QString coord0 = coords.mid(0, sep1);
            QString coord1 = coords.mid(sep1+1, sep2-sep1-1);
            QString coord2 = coords.mid(sep2+1);

            v0 = vectorFromVmfCoord(coord0);
            v1 = vectorFromVmfCoord(coord1);
            v2 = vectorFromVmfCoord(coord2);
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

        QMatrix4x4 matrixOrientation(const EulerAngle &angle)
        {
            return matrixRotateZ(qDegreesToRadians(angle.yaw()))
                    * matrixRotateY(qDegreesToRadians(angle.pitch()))
                    * matrixRotateX(qDegreesToRadians(angle.roll()));
        }
    }
}
