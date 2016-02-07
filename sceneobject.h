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
        Translucent = 0x1,
    };

    explicit SceneObject(SceneObject *parent = 0);
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

    int renderFlags() const;
    void setRenderFlags(int flags);

    // Computes bounds in local space.
    BoundingBox computeLocalBounds() const;

signals:

public slots:

protected:
    explicit SceneObject(const SceneObject &cloneFrom);
    QScopedPointer<GeometryData>    m_pGeometry;
    BaseScene*  m_pScene;
    int m_iRenderFlags;
};

#endif // SCENEOBJECT_H
