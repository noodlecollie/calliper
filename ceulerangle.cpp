#include "ceulerangle.h"

CEulerAngle::CEulerAngle()
{
    m_flPitch = 0;
    m_flYaw = 0;
    m_flRoll = 0;
}

CEulerAngle::CEulerAngle(float pitch, float yaw, float roll)
{
    m_flPitch = pitch;
    m_flYaw = yaw;
    m_flRoll = roll;
}

float CEulerAngle::pitch() const
{
    return m_flPitch;
}

void CEulerAngle::setPitch(float p)
{
    m_flPitch = p;
}

float CEulerAngle::yaw() const
{
    return m_flYaw;
}

void CEulerAngle::setYaw(float y)
{
    m_flYaw = y;
}

float CEulerAngle::roll() const
{
    return m_flRoll;
}

void CEulerAngle::setRoll(float r)
{
    m_flRoll = r;
}
