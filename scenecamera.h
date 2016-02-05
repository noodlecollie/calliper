#ifndef SCENECAMERA_H
#define SCENECAMERA_H

#include "sceneobject.h"
#include "cameralens.h"
#include <QScopedPointer>

class SceneCamera : public SceneObject
{
    Q_OBJECT
public:
    SceneCamera(SceneObject* parent = 0);
    virtual ~SceneCamera();

    CameraLens* lens() const;

    bool drawBounds() const;
    void setDrawBounds(bool enabled);

    // The world -> camera transform is parentToLocal().

    virtual void draw(ShaderStack *stack);
    virtual bool scalable() const;

    // The translation is defined from p0 to p1, along the plane that is parallel to the
    // camera view plane and at the specified distance in front of where the camera is pointing.
    QVector3D worldTranslation(const QPoint &p0, const QPoint &p1, const QSize &viewSize, float distFromCamera) const;

protected:
    virtual void rebuildLocalToParent() const;
    virtual void clampAngles();

private:
    void rebuildViewBoundsGeometry();

    QScopedPointer<CameraLens>  m_pLens;
    BoundingBox m_LocalLensBounds;
    QScopedPointer<GeometryData>   m_pBoundsGeom;
    bool        m_bDrawBounds;
};

#endif // SCENECAMERA_H
