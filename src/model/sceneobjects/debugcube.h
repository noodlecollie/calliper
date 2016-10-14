#ifndef DEBUGCUBE_H
#define DEBUGCUBE_H

#include "model_global.h"
#include "scene/sceneobject.h"

namespace NS_MODEL
{
    class MODELSHARED_EXPORT DebugCube : public SceneObject
    {
        friend class Scene;
        Q_OBJECT
    public:
        float radius() const;
        void setRadius(float r);

        bool drawFrame() const;
        void setDrawFrame(bool draw);

    protected:
        DebugCube(const SceneObjectInitParams &initParams, SceneObject* parentObject);
        DebugCube(const DebugCube *cloneFrom, const SceneObjectInitParams &initParams);
        virtual ~DebugCube();

        virtual void bakeGeometry(NS_RENDERER::GeometryBuilder &builder) const override;

    private:
        void commonInit();

        float m_flRadius;
        bool m_bDrawFrame;
    };
}

#endif // DEBUGCUBE_H
