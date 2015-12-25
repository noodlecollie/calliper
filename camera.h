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

    // The world -> camera transform is parentToLocal().

protected:
    virtual void rebuildLocalToParent() const;
    virtual void clampAngles();

private:
    CameraLens  m_Lens;
};

#endif // CAMERA_H
