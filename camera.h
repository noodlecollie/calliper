#ifndef CAMERA_H
#define CAMERA_H

#include "sceneobject.h"
#include "cameralens.h"

class Camera : public SceneObject
{
public:
    Camera(SceneObject* parent = 0);

    CameraLens lens() const;
    void setLens(const CameraLens &lens);

private:
    CameraLens  m_Lens;
};

#endif // CAMERA_H
