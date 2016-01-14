#ifndef CAMERAWRAPPER_H
#define CAMERAWRAPPER_H

class HierarchicalObject;
class CameraLens;
class SceneCamera;

class CameraWrapper
{
public:
    CameraWrapper(HierarchicalObject* camera, CameraLens* lens);
    CameraWrapper(SceneCamera* camera);

    HierarchicalObject* camera();
    const HierarchicalObject* camera() const;

    CameraLens* cameraLens();
    const CameraLens* cameraLens() const;

private:
    HierarchicalObject* m_pCamera;
    CameraLens*         m_pCameraLens;
};

#endif // CAMERAWRAPPER_H
