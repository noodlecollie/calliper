#include "camera.h"
#include "callipermath.h"
#include <QtMath>

#define MAX_PITCH_DELTA 89.0f
#define MAX_ROLL_DELTA 180.0f

Camera::Camera(SceneObject *parent) : SceneObject(parent),
    m_Lens(CameraLens::Perspective)
{

}

CameraLens Camera::lens() const
{
    return m_Lens;
}

void Camera::setLens(const CameraLens &lens)
{
    m_Lens = lens;
}

void Camera::rebuildLocalToParent() const
{
    // To get from local (model) space to world space,
    // we perform transforms forward.
    // To get from world space to camera space we must
    // perform the camera transforms backward.

    // Our default orientation should be looking down X,
    // but the OpenGL camera by default looks down (Hammer's) Y.
    // This means we need to apply an orientation first so that we face down
    // X before we do our other transforms.
    static const QMatrix4x4 defaultRot = Math::matrixRotateZ(qDegreesToRadians(-90.0f));

    SceneObject::rebuildLocalToParent();
    m_matLocalToParent = m_matLocalToParent * defaultRot;
}

void Camera::clampAngles()
{
    // These are slightly different to normal because the camera
    // needs to avoid the pitch singularities.

    if ( m_angAngles.pitch() < -MAX_PITCH_DELTA )
        m_angAngles.setPitch(-MAX_PITCH_DELTA);

    else if ( m_angAngles.pitch() > MAX_PITCH_DELTA )
        m_angAngles.setPitch(MAX_PITCH_DELTA);

    if ( m_angAngles.roll() < -MAX_ROLL_DELTA )
        m_angAngles.setRoll(-MAX_ROLL_DELTA);

    else if ( m_angAngles.roll() > MAX_ROLL_DELTA )
        m_angAngles.setRoll(MAX_ROLL_DELTA);

    m_angAngles.setYaw(std::fmod(m_angAngles.yaw(), 360.0f));
}
