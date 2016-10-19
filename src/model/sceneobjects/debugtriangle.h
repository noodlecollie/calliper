#ifndef DEBUGTRIANGLE_H
#define DEBUGTRIANGLE_H

#include "model_global.h"
#include "scene/sceneobject.h"

namespace NS_MODEL
{
    class MODELSHARED_EXPORT DebugTriangle : public SceneObject
    {
        friend class Scene;
        Q_OBJECT
    public:

    protected:
        DebugTriangle(const SceneObjectInitParams& initParams, SceneObject* parentObject);
        DebugTriangle(const DebugTriangle* cloneFrom, const SceneObjectInitParams& initParams);
        virtual ~DebugTriangle();

        virtual void bakeGeometry(NS_RENDERER::GeometryBuilder &builder) const override;

    private:
        void commonInit();
    };
}

#endif // DEBUGTRIANGLE_H
