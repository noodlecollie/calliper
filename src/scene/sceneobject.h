#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "hierarchicalobject.h"
#include <QScopedPointer>
#include "geometrydata.h"
#include <QMatrix4x4>
#include "eulerangle.h"
#include "boundingbox.h"

class BaseScene;
class ShaderStack;

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
    explicit SceneObject(const QJsonObject &serialisedData, SceneObject* parent = 0);
    virtual ~SceneObject();

    // Object name is not cloned.
    virtual SceneObject* clone() const;

    SceneObject* parentObject() const;
    QList<SceneObject*> children() const;
    BaseScene* scene() const;

    // The SceneObject owns its geometry.
    // Any old geometry that is replaced will be deleted.
    GeometryData* geometry() const;
    void setGeometry(GeometryData* data);

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

    QScopedPointer<GeometryData>    m_pGeometry;
	BaseScene*						m_pScene;
    RenderFlags						m_RenderFlags;
	bool							m_bHidden;
    bool                            m_bSerialiseGeometry;

private:
    void initDefaults(SceneObject* parent);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(SceneObject::RenderFlags)

#endif // SCENEOBJECT_H
