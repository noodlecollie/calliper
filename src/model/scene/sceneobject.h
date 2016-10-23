#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "model_global.h"
#include <QObject>
#include "scene/hierarchystate.h"
#include "events/spatialconfigurationchange.h"
#include "geometry/geometrybuilder.h"
#include "sceneobjectinitparams.h"
#include "shaders/shaderpalette.h"

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
        Scene* parentScene() const;

        quint32 objectId() const;

        HierarchyState& hierarchy();
        const HierarchyState& hierarchy() const;
        virtual bool scalable() const;

        bool needsRendererUpdate() const;
        void rendererUpdate(const ShaderPalette &shaderPalette, NS_RENDERER::GeometryBuilder &builder) const;

        // Not cached, so could be expensive if called a lot.
        QMatrix4x4 rootToLocalMatrix() const;
        QMatrix4x4 localToRootMatrix() const;

        QList<SceneObject*> childSceneObjects() const;

    public slots:
        void flagNeedsRendererUpdate();

    protected:
        // SceneObject constructors must have a first parameter as init params
        // to be valid for use with Scene::createSceneObject().
        // Other arguments will be filled in by the variadic std::move.

        // Standard constructor for a new SceneObject.
        SceneObject(const SceneObjectInitParams &initParams, SceneObject* parentObject);

        // Clone - only called by Scene::cloneSceneObject().
        SceneObject(const SceneObject* cloneFrom, const SceneObjectInitParams &initParams);

        // Only Scenes are allowed to destroy SceneObjects.
        virtual ~SceneObject();

        virtual void customEvent(QEvent *event);
        virtual void bakeGeometry(const ShaderPalette &shaderPalette, NS_RENDERER::GeometryBuilder &builder) const;

        // Called by subclasses to convert hierarchy state to non-scalable.
        void updateScalableState(bool isScalable);

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
        HierarchyState* initHierarchyState(bool isScalable);

        Scene* const m_pParentScene;
        const quint32 m_iObjectId;
        HierarchyState* m_pHierarchy;
        mutable bool m_bNeedsRendererUpdate;
    };
}

#endif // SCENEOBJECT_H
