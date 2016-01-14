#include "camerawrapper.h"
#include "hierarchicalobject.h"
#include "cameralens.h"
#include "scenecamera.h"

CameraWrapper::CameraWrapper(HierarchicalObject *camera, CameraLens *lens)
{
    m_pCamera = camera;
    m_pCameraLens = lens;
}

CameraWrapper::CameraWrapper(SceneCamera *camera)
{
    m_pCamera = camera;
    m_pCameraLens = camera->lens();
}

HierarchicalObject* CameraWrapper::camera()
{
    return m_pCamera;
}

const HierarchicalObject* CameraWrapper::camera() const
{
    return m_pCamera;
}

CameraLens* CameraWrapper::cameraLens()
{
    return m_pCameraLens;
}

const CameraLens* CameraWrapper::cameraLens() const
{
    return m_pCameraLens;
}
