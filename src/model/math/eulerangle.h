#ifndef EULERANGLE_H
#define EULERANGLE_H

/*
 * The following specification is written to imitate how Euler angles work in Hammer.
 * The left-handed Hammer co-ordinate system is used: X points right, Z points up, Y points away.
 *
 * (0, 0, 0) points down the positive X axis, with the up vector pointing up the positive vertical Z axis.
 * (-90, 0, 0) points straight up the positive Z axis, with the up vector pointing down the negative X axis.
 * (90, 0, 0) points straight down the negative Z axis, with the up vector pointing up the positive X axis.
 * (0, 90, 0) points up the positive Y axis, with the up vector still pointing up the positive vertical Z axis.
 * (0, 0, 90) points up the positive X axis with the up vector pointing down the negative Y axis.
 * (0, 0, -90) points up the positive X axis with the up vector pointing up the positive Y axis.
 *
 * Therefore the pitch, yaw and roll can be thought of in the following way:
 *
 * Yaw should always be applied around the world Z axis. A yaw of 0 points down the world X axis; increasing the yaw
 * rotates anticlockwise around the world Z axis as viewed normally.
 *
 * Pitch is applied after yaw, from the object's perspective. Negative pitch values rotate upwards to point up the Z axis;
 * positive values rotate downwards.
 *
 * Roll is applied after pitch, from the object's perspective. Positive roll values rotate clockwise around the vector
 * specified by pitch and yaw; negative values rotate anticlockwise.
 *
 * Therefore, we can think of Euler angles in the following way:
 * - Each angle is specified as a clockwise number of degrees when looking along the
 *      specified axis in the positive direction.
 * - Since from the object's perspective the order of transformations is YPR, when rotating with matrices we
 *      perform them in the order RPY.
 */

#include "model_global.h"
#include <QtDebug>

namespace NS_MODEL
{
    class MODELSHARED_EXPORT EulerAngle
    {
    public:
        EulerAngle();
        EulerAngle(float pitch, float yaw, float roll);

        float pitch() const;
        void setPitch(float p);

        float yaw() const;
        void setYaw(float y);

        float roll() const;
        void setRoll(float r);

        inline bool operator ==(const EulerAngle &other) const
        {
            return m_flPitch == other.m_flPitch && m_flYaw == other.m_flYaw && m_flRoll == other.m_flRoll;
        }

        inline EulerAngle operator +(const EulerAngle &other) const
        {
            return EulerAngle(m_flPitch + other.m_flPitch, m_flYaw + other.m_flYaw, m_flRoll + other.m_flRoll);
        }

        inline EulerAngle operator -(const EulerAngle &other) const
        {
            return EulerAngle(m_flPitch - other.m_flPitch, m_flYaw - other.m_flYaw, m_flRoll - other.m_flRoll);
        }

        inline float& operator [](int index)
        {
            static float dummy = 0;

            switch (index)
            {
            case 0:
                return m_flPitch;

            case 1:
                return m_flYaw;

            case 2:
                return m_flRoll;

            default:
                Q_ASSERT_X(false, Q_FUNC_INFO, "Index is invalid!");
                return dummy;
            }
        }

        inline const float& operator [](int index) const
        {
            static float dummy = 0;

            switch (index)
            {
            case 0:
                return m_flPitch;

            case 1:
                return m_flYaw;

            case 2:
                return m_flRoll;

            default:
                Q_ASSERT_X(false, Q_FUNC_INFO, "Index is invalid!");
                return dummy;
            }
        }

    private:
        float   m_flPitch;
        float   m_flYaw;
        float   m_flRoll;
    };
}

QDebug& operator <<(QDebug &debug, const NS_MODEL::EulerAngle &angle);

#endif // EULERANGLE_H
