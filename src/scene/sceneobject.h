#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "hierarchicalobject.h"
#include "geometrydata.h"
#include <QMatrix4x4>
#include "eulerangle.h"
#include "boundingbox.h"
#include <QSharedPointer>
#include <QVector>
#include "iraydetectable.h"

class BaseScene;
class ShaderStack;

typedef QSharedPointer<GeometryData> GeometryDataPointer;

class SceneObject : public HierarchicalObject, public IRayDetectable
{
    Q_OBJECT
    friend class BaseScene;
public:
    enum RenderFlag
    {
        NoRenderFlag = 0x0,
        Translucent = 0x1,
    };
    Q_DECLARE_FLAGS(RenderFlags, RenderFlag)
    Q_FLAG(RenderFlags)

    // Used for filtering different types of scene object.
    enum ObjectMask
    {
        AllObjectsMask = 0xffffffff,                            // Allow all.

        NotEditableMask = 0x1,                                  // Allow non-editables.
        EditableOnlyMask = AllObjectsMask & ~NotEditableMask    // Disallow non-editables.
    };

    SceneObject* parentObject() const;
    QList<SceneObject*> children() const;
    QList<SceneObject*> allChildren() const;
    QList<SceneObject*> ancestors() const;
    BaseScene* scene() const;
    bool isRoot() const;

    int geometryCount() const;
    GeometryDataPointer geometryAt(int index) const;
    void setGeometryAt(int index, const GeometryDataPointer &geom);
    void appendGeometry(const GeometryDataPointer &geom);
    void removeGeometry(int index);
    void clearGeometry();

    // Convenience.
    inline void appendGeometry(GeometryData* geom)
    {
        appendGeometry(GeometryDataPointer(geom));
    }

    // Empty => This object has no geometry
    // (=> it's just for grouping children)
    bool isEmpty() const;

    virtual bool editable() const;
    virtual void draw(ShaderStack* stack);
    void drawBoundsGeometry(ShaderStack* stack);

    // Assumed to be constant throughout the object's lifetime.
    // If true, object will be rendered in a pre-pass, and will
    // not affect depth for any objects in the main pass.
    virtual bool isBackground() const;

    RenderFlags renderFlags() const;
    void setRenderFlags(RenderFlags flags);

    // Computes bounds in local space.
    virtual BoundingBox computeLocalBounds() const;

    // Hierarchical bounds, returned in local space.
    BoundingBox hierarchicalBounds(bool forceRecompute = false) const;
    bool useCachedBounds() const;
    void setUseCachedBounds(bool use);
    void flagBoundsStale();

	bool hidden() const;
	void setHidden(bool hide);

    bool drawBounds() const;
    void setDrawBounds(bool draw);

    virtual bool serialiseToJson(QJsonObject &obj) const;
    virtual QString serialiseIdentifier() const;

    // False by default.
    bool shouldSerialiseGeometry() const;
    void setShouldSerialiseGeometry(bool enabled);

    virtual bool computeIntersection(const Ray3D &ray, RayTraceContact &contact, RayCoordinateSpace space) const;

    // Assumes the ray is in parent space.
    SceneObject* computeRayCastRecursive(const Ray3D &ray, RayTraceContact& contact);

    virtual bool passesObjectMask(int mask) const;

signals:

public slots:

protected:
    explicit SceneObject(BaseScene* scene, SceneObject *parent);
    SceneObject(BaseScene* scene, const QJsonObject &serialisedData, SceneObject* parent);
    SceneObject(const SceneObject &cloneFrom);
    virtual ~SceneObject();

    void drawGeometry(GeometryData* geom, ShaderStack* stack, const int* sections = NULL, int sectionCount = 0);

	BaseScene*						m_pScene;
    RenderFlags						m_RenderFlags;
	bool							m_bHidden;
    bool                            m_bSerialiseGeometry;
    QVector<GeometryDataPointer>    m_GeometryList;

private slots:
    void onOrientationChanged();

private:
    void initDefaults(SceneObject* parent, BaseScene* scene);
    void initDefaults();
    void deepCloneGeometryFrom(const QVector<GeometryDataPointer> &list);
    BoundingBox totalGeometryBounds() const;
    void serialiseAllGeometry(QJsonObject &obj) const;
    void unserialiseAllGeometry(const QJsonArray &geomArray);
    void unionOfChildBounds(BoundingBox& bbox) const;
    void updateCachedBounds() const;

    mutable BoundingBox             m_CachedBounds;
    mutable bool                    m_bBoundsStale;
    bool                            m_bUseCachedBounds;

    mutable QScopedPointer<GeometryData>    m_pBoundsGeom;
    bool                                    m_bDrawBounds;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(SceneObject::RenderFlags)

#endif // SCENEOBJECT_H
