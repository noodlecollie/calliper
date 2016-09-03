#ifndef CROSSHAIR3D_H
#define CROSSHAIR3D_H

#include "model_global.h"
#include "sceneobject.h"

namespace NS_MODEL
{
    class MODELSHARED_EXPORT Crosshair3D : public SceneObject
    {
        Q_OBJECT
        friend class BaseScene;
    public:
        virtual ObjectFlags objectFlags() const;

    protected:
        explicit Crosshair3D(BaseScene* scene, SceneObject *parent);
        Crosshair3D(const Crosshair3D &cloneFrom);
        virtual ~Crosshair3D();

    private:
        void constructGeometry();
    };
}

#endif // CROSSHAIR3D_H
