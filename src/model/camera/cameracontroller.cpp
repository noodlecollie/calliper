#include "cameracontroller.h"
#include <QtDebug>

namespace
{
    const int TIMER_INTERVAL = 1000/60;

    const float MAX_PITCH_DELTA = 89.0f;
    const float MAX_ROLL_DELTA = 179.0f;

    void setAnglesAndClamp(NS_MODEL::EulerAngle& angles, float delta, int index, float limit)
    {
        float newAngle = angles[index] + delta;

        if ( delta < 0.0f )
        {
            if ( newAngle < -limit )
                newAngle = -limit;
        }
        else
        {
            if ( newAngle > limit )
                newAngle = limit;
        }

        angles[index] = newAngle;
    }
}

namespace NS_MODEL
{
    CameraController::CameraController(QObject* parent) : QObject(parent),
        m_pCamera(nullptr), m_flForwardSpeed(100), m_flStrafeSpeed(100),
        m_iForwardState(NoMovement), m_iStrafeState(NoMovement)
    {
        m_Timer.setSingleShot(false);
        m_Timer.setInterval(TIMER_INTERVAL);
        connect(&m_Timer, &QTimer::timeout, this, &CameraController::tick);
    }

    SceneCamera* CameraController::camera() const
    {
        return m_pCamera;
    }

    void CameraController::setCamera(SceneCamera *camera)
    {
        m_pCamera = camera;
    }

    float CameraController::forwardSpeed() const
    {
        return m_flForwardSpeed;
    }

    void CameraController::setForwardSpeed(float speed)
    {
        m_flForwardSpeed = speed;
    }

    float CameraController::strafeSpeed() const
    {
        return m_flStrafeSpeed;
    }

    void CameraController::setStrafeSpeed(float speed)
    {
        m_flStrafeSpeed = speed;
    }

    bool CameraController::enabled() const
    {
       return m_Timer.isActive();
    }

    void CameraController::setEnabled(bool enabled)
    {
        if ( m_Timer.isActive() == enabled )
           return;

        if ( enabled )
        {
           m_Timer.start();
        }
        else
        {
            m_Timer.stop();
        }
    }

    void CameraController::tick()
    {
        if ( !m_pCamera )
            return;

        float secondFrac = 1.0f/static_cast<float>(TIMER_INTERVAL);

        float fwdThisTick = m_iForwardState * m_flForwardSpeed * secondFrac;
        float strafeThisTick = m_iStrafeState * m_flStrafeSpeed * secondFrac;

        QVector3D vecFwd(fwdThisTick, 0, 0);
        QVector3D vecStrafe(0, strafeThisTick, 0);
        QVector3D currentPos = m_pCamera->hierarchy().position();
        currentPos += vecFwd + vecStrafe;
        m_pCamera->hierarchy().setPosition(currentPos);
    }

    void CameraController::modifyMovementState(MovementState& member, bool active)
    {
        if ( active )
        {
            if ( member < PositiveMovement )
            {
                member = static_cast<MovementState>(member+1);
            }
        }
        else
        {
            if ( member > NegativeMovement )
            {
                member = static_cast<MovementState>(member-1);
            }
        }
    }

    void CameraController::moveForward(bool active)
    {
        modifyMovementState(m_iForwardState, active);
    }

    void CameraController::moveBackward(bool active)
    {
        modifyMovementState(m_iForwardState, !active);
    }

    void CameraController::moveLeft(bool active)
    {
        modifyMovementState(m_iStrafeState, active);
    }

    void CameraController::moveRight(bool active)
    {
        modifyMovementState(m_iStrafeState, !active);
    }

    void CameraController::clearMovement()
    {
        m_iForwardState = NoMovement;
        m_iStrafeState = NoMovement;
    }

    void CameraController::addPitch(float pitch)
    {
        if ( !m_pCamera )
            return;

        EulerAngle angles = m_pCamera->hierarchy().rotation();
        setAnglesAndClamp(angles, pitch, 0, MAX_PITCH_DELTA);
        m_pCamera->hierarchy().setRotation(angles);
    }

    void CameraController::addYaw(float yaw)
    {
        if ( !m_pCamera )
            return;

        EulerAngle angles = m_pCamera->hierarchy().rotation();
        float newYaw = angles.yaw() + yaw;

        while ( newYaw >= 360.0f )
            newYaw -= 360.0f;

        while ( newYaw < 0.0f )
            newYaw += 360.0f;

        angles.setYaw(newYaw);
        m_pCamera->hierarchy().setRotation(angles);
    }

    void CameraController::addRoll(float roll)
    {
        if ( !m_pCamera )
            return;

        EulerAngle angles = m_pCamera->hierarchy().rotation();
        setAnglesAndClamp(angles, roll, 2, MAX_ROLL_DELTA);
        m_pCamera->hierarchy().setRotation(angles);
    }

    void CameraController::debugIncrementPitch(bool pressed)
    {
        addPitch(pressed ? 10 : -10);
    }
}
