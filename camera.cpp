#include "camera.h"
#include "callipermath.h"
#include <QtMath>

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

void Camera::rebuildMatrices() const
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

    m_matLocalToParent = Math::matrixTranslate(m_vecPosition) * defaultRot;
    m_matParentToLocal = m_matLocalToParent.inverted();
    m_bMatricesStale = false;
}
