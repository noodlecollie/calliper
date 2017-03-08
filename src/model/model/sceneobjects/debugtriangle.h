#ifndef DEBUGTRIANGLE_H
#define DEBUGTRIANGLE_H

#include "model_global.h"
#include "model/scene/sceneobject.h"

namespace Model
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

        virtual void bakeGeometry(const Renderer::ShaderPalette &shaderPalette, Renderer::GeometryBuilder &builder) const;

    private:
        void commonInit();
    };
}

#endif // DEBUGTRIANGLE_H
