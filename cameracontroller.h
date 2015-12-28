#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <QVector3D>

class CameraController
{
public:
    CameraController();

    enum MovementFlags
    {
        NoMovement = 0x0,
        MoveForward = 0x1,
        MoveBackward = 0x2,
        MoveRight = 0x4,
        MoveLeft = 0x8,
        MoveUp = 0x10,
        MoveDown = 0x20
    };

    float topSpeed() const;
    void setTopSpeed(float speed);
    float accelTime() const;
    void setAccelTime(float seconds);
    float decelTime() const;
    void setDecelTime(float seconds);
    QVector3D velocity() const;

    void forward(bool pressed);
    void backward(bool pressed);
    void left(bool pressed);
    void right(bool pressed);
    void up(bool pressed);
    void down(bool pressed);
    void clearMovementFlags();

    void update(int ms);
    void reset();

private:
    QVector3D vectorFromMovementFlags();

    float       m_flTopSpeed;
    float       m_flAccelTime;
    float       m_flDecelTime;
    int         m_iMovementFlags;
    QVector3D   m_vecVelocity;
};

#endif // CAMERACONTROLLER_H
