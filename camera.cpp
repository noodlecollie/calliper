#include "camera.h"

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
