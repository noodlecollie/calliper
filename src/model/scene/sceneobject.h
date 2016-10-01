#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "model_global.h"
#include <QObject>
#include "scene/hierarchyparams.h"

namespace NS_MODEL
{
    class Scene;

    class SceneObject : public QObject
    {
        friend class Scene;
        Q_OBJECT
    public:
        virtual ~SceneObject();
        SceneObject* parentObject() const;
        bool isRoot() const;

        HierarchyParams& hierarchy();
        const HierarchyParams& hierarchy() const;

    protected:
        // SceneObject constructors must have a first parameter as a parent Scene pointer
        // to be valid for use with Scene::createSceneObject().
        // Other arguments will be filled in by the variadic std::move.

        // Standard constructor for a new SceneObject.
        SceneObject(Scene* parentScene, SceneObject* parentObject);

        // Clone - only called by Scene::cloneSceneObject().
        explicit SceneObject(const SceneObject* cloneFrom);

    private slots:
        void onOwnPositionChanged();
        void onOwnRotationChanged();
        void onOwnScaleChanged();

    private:
        // SceneObjects should not be copyable.
        SceneObject(const SceneObject &other) = delete;
        SceneObject& operator =(const SceneObject &other) = delete;

        void commonInit();

        HierarchyParams* m_pHierarchy;
    };
}

#endif // SCENEOBJECT_H
