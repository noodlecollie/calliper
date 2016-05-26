#ifndef CROSSHAIR3D_H
#define CROSSHAIR3D_H

#include "sceneobject.h"

class Crosshair3D : public SceneObject
{
    Q_OBJECT
public:

protected:
    explicit Crosshair3D(BaseScene* scene, SceneObject *parent);
    Crosshair3D(const Crosshair3D &cloneFrom);
    virtual ~Crosshair3D();

private:
    void constructGeometry();
};

#endif // CROSSHAIR3D_H
