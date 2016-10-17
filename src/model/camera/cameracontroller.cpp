#include "cameracontroller.h"

namespace
{
    const int TIMER_INTERVAL = 1000/60;
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
}
