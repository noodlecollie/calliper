#include "cameracontroller.h"

CameraController::CameraController()
{
    m_iMovementFlags = NoMovement;
    m_flTopSpeed = 1000.0f;
    m_flAccelTime = 0.5f;
    m_flDecelTime = 0.2f;
}

float CameraController::topSpeed() const
{
    return m_flTopSpeed;
}

void CameraController::setTopSpeed(float speed)
{
    m_flTopSpeed = speed;
}

float CameraController::accelTime() const
{
    return m_flAccelTime;
}

void CameraController::setAccelTime(float seconds)
{
    m_flAccelTime = seconds;
}

float CameraController::decelTime() const
{
    return m_flDecelTime;
}

void CameraController::setDecelTime(float seconds)
{
    m_flDecelTime = seconds;
}

QVector3D CameraController::vectorFromMovementFlags()
{
    int x = 0, y = 0, z = 0;

    if ( (m_iMovementFlags & MoveForward) == MoveForward )
    {
        x++;
    }

    if ( (m_iMovementFlags & MoveBackward) == MoveBackward )
    {
        x--;
    }

    if ( (m_iMovementFlags & MoveLeft) == MoveLeft )
    {
        y++;
    }

    if ( (m_iMovementFlags & MoveRight) == MoveRight )
    {
        y--;
    }

    if ( (m_iMovementFlags & MoveUp) == MoveUp )
    {
        z++;
    }

    if ( (m_iMovementFlags & MoveDown) == MoveDown  )
    {
        z--;
    }

    return QVector3D(x,y,z).normalized();
}

void CameraController::clearMovementFlags()
{
    m_iMovementFlags = NoMovement;
}

void CameraController::forward(bool pressed)
{
    if (pressed)
        m_iMovementFlags |= MoveForward;
    else
        m_iMovementFlags &= ~MoveForward;
}

void CameraController::backward(bool pressed)
{
    if (pressed)
        m_iMovementFlags |= MoveBackward;
    else
        m_iMovementFlags &= ~MoveBackward;
}

void CameraController::left(bool pressed)
{
    if (pressed)
        m_iMovementFlags |= MoveLeft;
    else
        m_iMovementFlags &= ~MoveLeft;
}

void CameraController::right(bool pressed)
{
    if (pressed)
        m_iMovementFlags |= MoveRight;
    else
        m_iMovementFlags &= ~MoveRight;
}

void CameraController::up(bool pressed)
{
    if (pressed)
        m_iMovementFlags |= MoveUp;
    else
        m_iMovementFlags &= ~MoveUp;
}

void CameraController::down(bool pressed)
{
    if (pressed)
        m_iMovementFlags |= MoveDown;
    else
        m_iMovementFlags &= ~MoveDown;
}

void CameraController::update(int ms)
{
    if ( ms < 1 )
        return;

    // In any axes where no buttons are pressed (ie. where the axis value is zero), we want to decelerate back towards zero speed.
    // In axes where buttons are pressed, we want to accelerate in this direction.

    // Fraction of a second passed:
    float frac = (float)ms/1000.0f;

    // Calculate the acceleration and deceleration that should occur over the time that has elapsed.
    float dec = decelTime() <= 0.0f ? 0.0f : ((topSpeed()/decelTime()) * frac);
    float acc = accelTime() <= 0.0f ? 0.0f : ((topSpeed()/accelTime()) * frac);

    // Get the direction in which we should accelerate.
    QVector3D buttonDir = vectorFromMovementFlags();

    // Decelerate back towards zero in directions in which no buttons are pressed.
    // Make sure we also account for if dec is 0 - this means that we should stop immediately.
    if ( buttonDir.x() == 0.0f )
    {
        if ( m_vecVelocity.x() > 0.0f )
        {
            m_vecVelocity.setX(dec > 0.0f ? qMax(m_vecVelocity.x() - dec, 0.0f) : 0.0f);
        }
        else if ( m_vecVelocity.x() < 0.0f )
        {
            m_vecVelocity.setX(dec > 0.0f ? qMin(m_vecVelocity.x() + dec, 0.0f) : 0.0f);
        }
    }

    if ( buttonDir.y() == 0.0f )
    {
        if ( m_vecVelocity.y() > 0.0f )
        {
            m_vecVelocity.setY(dec > 0.0f ? qMax(m_vecVelocity.y() - dec, 0.0f) : 0.0f);
        }
        else if ( m_vecVelocity.y() < 0.0f )
        {
            m_vecVelocity.setY(dec > 0.0f ? qMin(m_vecVelocity.y() + dec, 0.0f) : 0.0f);
        }
    }

    if ( buttonDir.z() == 0.0f )
    {
        if ( m_vecVelocity.z() > 0.0f )
        {
            m_vecVelocity.setZ(dec > 0.0f ? qMax(m_vecVelocity.z() - dec, 0.0f) : 0.0f);
        }
        else if ( m_vecVelocity.z() < 0.0f )
        {
            m_vecVelocity.setZ(dec > 0.0f ? qMin(m_vecVelocity.z() + dec, 0.0f) : 0.0f);
        }
    }

    // Add the motion vector from the buttons that are pressed to the current speed.
    // If acc is zero, just set velocity to the max speed.
    m_vecVelocity += buttonDir * (acc > 0.0f ? acc : topSpeed());
}

QVector3D CameraController::velocity() const
{
    return m_vecVelocity;
}
