#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include "model_global.h"
#include "scenecamera.h"
#include <QTimer>
#include <QObject>

namespace NS_MODEL
{
    class MODELSHARED_EXPORT CameraController : public QObject
    {
    public:
        CameraController(QObject* parent = 0);

        SceneCamera* camera() const;
        void setCamera(SceneCamera* camera);

        float forwardSpeed() const;
        void setForwardSpeed(float speed);

        float strafeSpeed() const;
        void setStrafeSpeed(float speed);

        float verticalSpeed() const;
        void setVerticalSpeed(float speed);

        bool enabled() const;
        void setEnabled(bool enabled);

    public slots:
        void moveForward(bool active);
        void moveBackward(bool active);
        void moveLeft(bool active);
        void moveRight(bool active);
        void moveUp(bool active);
        void moveDown(bool active);
        void clearMovement();

        void addPitch(float pitch);
        void addYaw(float yaw);
        void addRoll(float roll);

    private slots:
        void tick();

    private:
        enum MovementState
        {
            PositiveMovement = 1,
            NoMovement = 0,
            NegativeMovement = -1,
        };

        static void modifyMovementState(MovementState& member, bool active);

        SceneCamera* m_pCamera;
        float   m_flForwardSpeed;
        float   m_flStrafeSpeed;
        float   m_flVerticalSpeed;
        MovementState m_iForwardState;
        MovementState m_iStrafeState;
        MovementState m_iVerticalState;

        QTimer m_Timer;
    };
}

#endif // CAMERACONTROLLER_H
