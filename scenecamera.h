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

    // Returns the given pixel point as a 3D vector point on the near plane.
    QVector3D mapPoint(const QPoint &pos, const QSize &viewSize) const;

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
