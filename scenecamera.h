#ifndef SCENECAMERA_H
#define SCENECAMERA_H

#include "sceneobject.h"
#include "cameralens.h"
#include <QScopedPointer>

class SceneCamera : public SceneObject
{
    Q_OBJECT
    friend class UnserialisationFactory;
public:
    explicit SceneCamera(SceneObject* parent = 0);
    virtual ~SceneCamera();
    virtual SceneObject* clone() const;

    CameraLens* lens() const;

    bool drawBounds() const;
    void setDrawBounds(bool enabled);

    // The world -> camera transform is parentToLocal().

    virtual void draw(ShaderStack *stack);
    virtual bool scalable() const;

    // The translation is defined from p0 to p1, along the plane that is parallel to the
    // camera view plane and at the specified distance in front of where the camera is pointing.
    QVector3D worldTranslation(const QPoint &p0, const QPoint &p1, const QSize &viewSize, float distFromCamera) const;

    // Given a point on the screen, returns the direction of a ray cast from the camera origin that goes through that point.
    // This is a world direction.
    QVector3D frustumDirection(const QPoint &p, const QSize &viewSize) const;

    virtual bool serialiseToJson(QJsonObject &obj) const;
    virtual QString serialiseIdentifier() const;

protected:
    explicit SceneCamera(const SceneCamera &cloneFrom);
    explicit SceneCamera(const QJsonObject &serialisedData, SceneObject* parent = 0);
    virtual void rebuildLocalToParent() const;
    virtual void clampAngles();

private:
    void initDefaults();
    void rebuildViewBoundsGeometry();

    QScopedPointer<CameraLens>  m_pLens;
    BoundingBox m_LocalLensBounds;
    QScopedPointer<GeometryData>   m_pBoundsGeom;
    bool        m_bDrawBounds;
};

#endif // SCENECAMERA_H
