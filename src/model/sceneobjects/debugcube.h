#ifndef DEBUGCUBE_H
#define DEBUGCUBE_H

#include "model_global.h"
#include "scene/sceneobject.h"

namespace NS_MODEL
{
    class MODELSHARED_EXPORT DebugCube : public SceneObject
    {
        friend class Scene;
    public:
        float radius() const;
        void setRadius(float r);

    protected:
        DebugCube(Scene* parentScene, SceneObject* parentObject);
        explicit DebugCube(const DebugCube* cloneFrom);
        virtual ~DebugCube();

        virtual void bakeGeometry(NS_RENDERER::GeometryBuilder &builder) const;

        bool drawFrame() const;
        void setDrawFrame(bool draw);

    private:
        void commonInit();

        float m_flRadius;
        bool m_bDrawFrame;
    };
}

#endif // DEBUGCUBE_H
