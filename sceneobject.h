#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "hierarchicalobject.h"
#include <QScopedPointer>
#include "geometrydata.h"
#include <QMatrix4x4>
#include "eulerangle.h"

class Scene;
class ShaderStack;

class SceneObject : public HierarchicalObject
{
    Q_OBJECT
    friend class Scene;
public:
    explicit SceneObject(SceneObject *parent = 0);
    virtual ~SceneObject();

    SceneObject* parentObject() const;
    QList<SceneObject*> children() const;
    Scene* scene() const;

    // The SceneObject owns its geometry.
    // Any old geometry that is replaced will be deleted.
    GeometryData* geometry() const;
    void setGeometry(GeometryData* data);

    // Empty => This object has no geometry
    // (=> it's just for grouping children)
    bool isEmpty() const;

    bool ignoreDepth() const;
    void setIgnoreDepth(bool ignore);

    virtual bool editable() const;
    virtual void draw(ShaderStack* stack);

signals:

public slots:

protected:
    QScopedPointer<GeometryData>    m_pGeometry;

    bool    m_bIgnoreDepth;
    Scene*  m_pScene;
};

#endif // SCENEOBJECT_H
