#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include "model_global.h"
#include "scenecamera.h"
#include <QTimer>
#include <QObject>

namespace NS_MODEL
{
    class CameraController : public QObject
    {
    public:
        CameraController(QObject* parent = 0);

        SceneCamera* camera() const;
        void setCamera(SceneCamera* camera);

        float forwardSpeed() const;
        void setForwardSpeed(float speed);

        float strafeSpeed() const;
        void setStrafeSpeed(float speed);

        bool enabled() const;
        void setEnabled(bool enabled);

    public slots:
        void moveForward(bool active);
        void moveBackward(bool active);
        void moveLeft(bool active);
        void moveRight(bool active);

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
        MovementState m_iForwardState;
        MovementState m_iStrafeState;

        QTimer m_Timer;
    };
}

#endif // CAMERACONTROLLER_H
