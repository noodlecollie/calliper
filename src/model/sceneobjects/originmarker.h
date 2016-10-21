#ifndef ORIGINMARKER_H
#define ORIGINMARKER_H

#include "model_global.h"
#include "scene/sceneobject.h"

namespace NS_MODEL
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

        virtual void bakeGeometry(NS_RENDERER::GeometryBuilder &builder) const override;

    private:
        void commonInit();
    };
}

#endif // ORIGINMARKER_H
