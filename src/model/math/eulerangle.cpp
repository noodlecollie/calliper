#include "eulerangle.h"

namespace NS_MODEL
{
    QDebug operator <<(QDebug debug, const EulerAngle &angle)
    {
        QDebugStateSaver stateSaver(debug);
        debug.nospace() << "EulerAngle(" << angle.pitch() << "," << angle.yaw() << "," << angle.roll() << ")";
        return debug;
    }

    EulerAngle::EulerAngle()
    {
        m_flPitch = 0;
        m_flYaw = 0;
        m_flRoll = 0;
    }

    EulerAngle::EulerAngle(float pitch, float yaw, float roll)
    {
        m_flPitch = pitch;
        m_flYaw = yaw;
        m_flRoll = roll;
    }

    float EulerAngle::pitch() const
    {
        return m_flPitch;
    }

    void EulerAngle::setPitch(float p)
    {
        m_flPitch = p;
    }

    float EulerAngle::yaw() const
    {
        return m_flYaw;
    }

    void EulerAngle::setYaw(float y)
    {
        m_flYaw = y;
    }

    float EulerAngle::roll() const
    {
        return m_flRoll;
    }

    void EulerAngle::setRoll(float r)
    {
        m_flRoll = r;
    }
}
