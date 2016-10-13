#include "scenecamera.h"

namespace NS_MODEL
{
    SceneCamera::SceneCamera(const SceneObjectInitParams &initParams, SceneObject* parentObject)
        : SceneObject(initParams, parentObject), m_Lens(CameraLens::Perspective)
    {
        commonInit();
    }

    SceneCamera::SceneCamera(const SceneCamera* cloneFrom, const SceneObjectInitParams &initParams)
        : SceneObject(cloneFrom, initParams), m_Lens(cloneFrom->lens())
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