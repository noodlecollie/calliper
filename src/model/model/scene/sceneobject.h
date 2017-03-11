#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "model_global.h"
#include <QObject>
#include "model/scene/hierarchystate.h"
#include "model/events/spatialconfigurationchange.h"
#include "renderer/geometry/geometrybuilder.h"
#include "sceneobjectinitparams.h"
#include "renderer/shaders/baseshaderpalette.h"
#include <QColor>

namespace Model
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

        // Sets the object as the parent of this object,
        // and adjusts this object's transformations so
        // that its translation/rotation/scale are maintained.
        void setParentObject(SceneObject* newParent);

        quint32 objectId() const;

        HierarchyState& hierarchy();
        const HierarchyState& hierarchy() const;
        virtual bool scalable() const;

        bool needsRendererUpdate() const;
        void rendererUpdate(Renderer::GeometryBuilder &builder) const;

        // Not cached, so could be expensive if called a lot.
        QMatrix4x4 rootToLocalMatrix() const;
        QMatrix4x4 localToRootMatrix() const;

        QList<SceneObject*> childSceneObjects() const;

        QColor color() const;
        void setColor(const QColor &col);

        bool mustExist() const;
        void setMustExist(bool mustExist);

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
        virtual void bakeGeometry(Renderer::GeometryBuilder &builder) const;

        // Called by subclasses to convert hierarchy state to non-scalable.
        void updateScalableState(bool isScalable);

        // Does the subclass override the vertex colours?
        // If not, colour attributes in the geometry builder are
        // assumed to be 4D vectors, match the number of positional
        // items, and be the same colour as the object's colour.
        virtual bool customVertexColours() const;

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
        void updateGeometryColours(Renderer::GeometryBuilder &builder) const;
        void updateGeometryColours(Renderer::GeometrySection* section) const;

        Scene* const m_pParentScene;
        const quint32 m_iObjectId;

        HierarchyState* m_pHierarchy;
        mutable bool m_bNeedsRendererUpdate;
        QColor m_colColor;
        bool m_bMustExist;
    };
}

#endif // SCENEOBJECT_H
