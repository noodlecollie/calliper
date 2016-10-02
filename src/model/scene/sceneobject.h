#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "model_global.h"
#include <QObject>
#include "scene/hierarchystate.h"
#include "events/spatialconfigurationchange.h"
#include "geometry/geometrybuilder.h"

namespace NS_MODEL
{
    class Scene;

    class MODELSHARED_EXPORT SceneObject : public QObject
    {
        friend class Scene;
        Q_OBJECT
    public:
        SceneObject* parentObject() const;
        bool isRoot() const;

        HierarchyState& hierarchy();
        const HierarchyState& hierarchy() const;

        bool needsRendererUpdate() const;
        void flagNeedsRendererUpdate();
        void rendererUpdate(NS_RENDERER::GeometryBuilder &builder) const;

    protected:
        // SceneObject constructors must have a first parameter as a parent Scene pointer
        // to be valid for use with Scene::createSceneObject().
        // Other arguments will be filled in by the variadic std::move.

        // Standard constructor for a new SceneObject.
        SceneObject(Scene* parentScene, SceneObject* parentObject);

        // Clone - only called by Scene::cloneSceneObject().
        explicit SceneObject(const SceneObject* cloneFrom);

        // Only Scenes are allowed to destroy SceneObjects.
        virtual ~SceneObject();

        virtual void customEvent(QEvent *event);
        virtual void bakeGeometry(NS_RENDERER::GeometryBuilder &builder) const;

    private slots:
        void onOwnPositionChanged();
        void onOwnRotationChanged();
        void onOwnScaleChanged();

    private:
        // SceneObjects should not be copyable.
        SceneObject(const SceneObject &other) = delete;
        SceneObject& operator =(const SceneObject &other) = delete;

        void commonInit();
        void handleSpatialConfigurationChange(SpatialConfigurationChange* event);

        HierarchyState* m_pHierarchy;
        mutable bool m_bNeedsRendererUpdate;
    };
}

#endif // SCENEOBJECT_H
