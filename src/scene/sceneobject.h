#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "hierarchicalobject.h"
#include "geometrydata.h"
#include <QMatrix4x4>
#include "eulerangle.h"
#include "boundingbox.h"
#include <QSharedPointer>
#include <QVector>

class BaseScene;
class ShaderStack;

typedef QSharedPointer<GeometryData> GeometryDataPointer;

class SceneObject : public HierarchicalObject
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

    explicit SceneObject(SceneObject *parent = 0);
    SceneObject(const QJsonObject &serialisedData, SceneObject* parent = 0);
    virtual ~SceneObject();

    // Object name is not cloned.
    virtual SceneObject* clone() const;

    SceneObject* parentObject() const;
    QList<SceneObject*> children() const;
    BaseScene* scene() const;

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

    RenderFlags renderFlags() const;
    void setRenderFlags(RenderFlags flags);

    // Computes bounds in local space.
    BoundingBox computeLocalBounds() const;

	bool hidden() const;
	void setHidden(bool hide);

    virtual bool serialiseToJson(QJsonObject &obj) const;
    virtual QString serialiseIdentifier() const;

    // False by default.
    bool shouldSerialiseGeometry() const;
    void setShouldSerialiseGeometry(bool enabled);

signals:

public slots:

protected:
    explicit SceneObject(const SceneObject &cloneFrom);

	BaseScene*						m_pScene;
    RenderFlags						m_RenderFlags;
	bool							m_bHidden;
    bool                            m_bSerialiseGeometry;
    QVector<GeometryDataPointer>    m_GeometryList;

private:
    void initDefaults(SceneObject* parent);
    void deepCloneGeometryFrom(const QVector<GeometryDataPointer> &list);
    void drawGeometry(GeometryData* geom, ShaderStack* stack);
    BoundingBox totalGeometryBounds() const;
    void serialiseAllGeometry(QJsonObject &obj) const;
    void unserialiseAllGeometry(const QJsonArray &geomArray);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(SceneObject::RenderFlags)

#endif // SCENEOBJECT_H
