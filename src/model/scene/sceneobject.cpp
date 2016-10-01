#include "sceneobject.h"

namespace NS_MODEL
{
    SceneObject::SceneObject(Scene* parentScene, SceneObject* parentObject)
        : QObject(parentObject)
    {
        commonInit();
    }

    SceneObject::SceneObject(const SceneObject *cloneFrom)
        : QObject(cloneFrom->parentObject())
    {
        commonInit();
    }

    SceneObject::~SceneObject()
    {

    }

    void SceneObject::commonInit()
    {
        m_pHierarchy = new HierarchyParams(this);
        connect(m_pHierarchy, &HierarchyParams::positionChanged, this, &SceneObject::onOwnPositionChanged);
        connect(m_pHierarchy, &HierarchyParams::rotationChanged, this, &SceneObject::onOwnRotationChanged);
        connect(m_pHierarchy, &HierarchyParams::scaleChanged, this, &SceneObject::onOwnScaleChanged);
    }

    SceneObject* SceneObject::parentObject() const
    {
        return qobject_cast<SceneObject*>(parent());
    }

    bool SceneObject::isRoot() const
    {
        return parentObject() == NULL;
    }

    HierarchyParams& SceneObject::hierarchy()
    {
        return *m_pHierarchy;
    }

    const HierarchyParams& SceneObject::hierarchy() const
    {
        return *m_pHierarchy;
    }

    void SceneObject::onOwnPositionChanged()
    {

    }

    void SceneObject::onOwnRotationChanged()
    {

    }

    void SceneObject::onOwnScaleChanged()
    {

    }
}
