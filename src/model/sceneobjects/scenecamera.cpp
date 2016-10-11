#include "scenecamera.h"

namespace NS_MODEL
{
    SceneCamera::SceneCamera(Scene* parentScene, SceneObject* parentObject)
        : SceneObject(parentScene, parentObject), m_Lens(CameraLens::Perspective)
    {
        commonInit();
    }

    SceneCamera::SceneCamera(const SceneCamera *cloneFrom)
        : SceneObject(cloneFrom), m_Lens(cloneFrom->lens())
    {
        commonInit();
    }

    SceneCamera::~SceneCamera()
    {

    }

    void SceneCamera::commonInit()
    {
        // Set us not to be scalable.
        updateScalableState(scalable());
    }

    bool SceneCamera::scalable() const
    {
        return false;
    }

    CameraLens SceneCamera::lens() const
    {
        return m_Lens;
    }

    void SceneCamera::setLens(const CameraLens &lens)
    {
        m_Lens = lens;
    }
}
