#ifndef ORIGINMARKER_H
#define ORIGINMARKER_H

#include "model_global.h"
#include "model/scene/sceneobject.h"

namespace Model
{
    class MODELSHARED_EXPORT OriginMarker : public SceneObject
    {
        friend class Scene;
        Q_OBJECT
    public:
        virtual bool scalable() const override;

    protected:
        OriginMarker(const SceneObjectInitParams &initParams, SceneObject* parentObject);
        OriginMarker(const OriginMarker *cloneFrom, const SceneObjectInitParams &initParams);
        virtual ~OriginMarker();

        virtual void bakeGeometry(const ShaderPalette &shaderPalette, Renderer::GeometryBuilder &builder) const override;
        virtual bool customVertexColours() const override { return true; }

    private:
        void commonInit();
    };
}

#endif // ORIGINMARKER_H
