#ifndef CAMERA_H
#define CAMERA_H

#include "sceneobject.h"
#include "cameralens.h"
#include <QScopedPointer>

class Camera : public SceneObject
{
    Q_OBJECT
public:
    Camera(SceneObject* parent = 0);
    virtual ~Camera();

    CameraLens lens() const;
    void setLens(const CameraLens &lens);

    bool drawBounds() const;
    void setDrawBounds(bool enabled);

    // The world -> camera transform is parentToLocal().

    virtual void draw(ShaderStack *stack);

protected:
    virtual void rebuildLocalToParent() const;
    virtual void clampAngles();

private:
    void rebuildViewBoundsGeometry();

    CameraLens  m_Lens;
    BoundingBox m_LocalLensBounds;
    QScopedPointer<GeometryData>   m_pBoundsGeom;
    bool        m_bDrawBounds;
};

#endif // CAMERA_H
