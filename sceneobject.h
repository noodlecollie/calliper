#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "hierarchicalobject.h"
#include <QScopedPointer>
#include "geometrydata.h"
#include <QMatrix4x4>
#include "eulerangle.h"

class MapScene;
class ShaderStack;

class SceneObject : public HierarchicalObject
{
    Q_OBJECT
    friend class MapScene;
public:
    enum RenderFlag
    {
        IgnoreDepth = 0x1,
    };

    explicit SceneObject(SceneObject *parent = 0);
    virtual ~SceneObject();

    SceneObject* parentObject() const;
    QList<SceneObject*> children() const;
    MapScene* scene() const;

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

signals:

public slots:

protected:
    QScopedPointer<GeometryData>    m_pGeometry;
    MapScene*  m_pScene;
    int m_iRenderFlags;
};

#endif // SCENEOBJECT_H
