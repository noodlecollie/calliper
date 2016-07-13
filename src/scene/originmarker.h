#ifndef ORIGINMARKER_H
#define ORIGINMARKER_H

#include "sceneobject.h"

class OriginMarker : public SceneObject
{
    Q_OBJECT
    friend class BaseScene;
public:
    virtual bool editable() const;
    virtual BoundingBox computeLocalBounds() const;

protected:
    explicit OriginMarker(BaseScene* scene, SceneObject* parent);
    OriginMarker(const OriginMarker &cloneFrom);
    virtual ~OriginMarker();

private:
    void constructGeometry();
};

#endif // ORIGINMARKER_H
