#include "modelmath.h"
#include "math/math.h"
#include <QtMath>
#include "fuzzyvertexmap.h"

namespace NS_MODEL
{
    namespace ModelMath
    {
        QMatrix4x4 matrixOrientation(const EulerAngle &angle)
        {
            return NS_CALLIPERUTIL::Math::matrixRotateZ(qDegreesToRadians(angle.yaw()))
                    * NS_CALLIPERUTIL::Math::matrixRotateY(qDegreesToRadians(angle.pitch()))
                    * NS_CALLIPERUTIL::Math::matrixRotateX(qDegreesToRadians(angle.roll()));
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

        QList<QVector3D> windingsToVertices(QList<Winding3D*> &windings)
        {
            typedef QLinkedList<WindingVertex> WindingVertexList;
            FuzzyVertexMap vertexMap;

            for ( int i = 0; i < windings.count(); i++ )
            {
                Winding3D* winding = windings[i];
                for ( WindingVertexList::iterator it = winding->verticesBegin(); it != winding->verticesEnd(); ++it )
                {
                    int index = vertexMap.mapToIndex(it->position());
                    it->setIndex(index);
                }
            }

            return vertexMap.vertexList();
        }

        void clipWindingsWithEachOther(QList<Winding3D *> windings)
        {
            QList<Plane3D> planes;
            for ( int i = 0; i < windings.count(); i++ )
            {
                planes.append(windings.at(i)->plane());
            }

            for ( int i = 0; i < windings.count(); i++ )
            {
                // Cache our plane so we can set it to null in the list.
                Plane3D tempPlane = planes.at(i);
                planes[i] = Plane3D();

                // Clip the winding by the remaining planes.
                windings[i]->clip(planes);

                // Reset the plane.
                planes[i] = tempPlane;
            }
        }
    }
}
