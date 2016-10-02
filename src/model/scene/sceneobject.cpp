#include "sceneobject.h"
#include <QCoreApplication>
#include "events/modeleventtypes.h"

namespace NS_MODEL
{
    SceneObject::SceneObject(Scene* parentScene, SceneObject* parentObject)
        : QObject(parentObject)
    {
        Q_ASSERT_X(parentScene, Q_FUNC_INFO, "Must have a valid parent scene!");
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
        m_pHierarchy = new HierarchyState(this);
        connect(m_pHierarchy, &HierarchyState::positionChanged, this, &SceneObject::onOwnPositionChanged);
        connect(m_pHierarchy, &HierarchyState::rotationChanged, this, &SceneObject::onOwnRotationChanged);
        connect(m_pHierarchy, &HierarchyState::scaleChanged, this, &SceneObject::onOwnScaleChanged);
    }

    void SceneObject::customEvent(QEvent *event)
    {
        switch(event->type())
        {
        case SpatialConfigurationChangeEvent:
            handleSpatialConfigurationChange(static_cast<SpatialConfigurationChange*>(event));
            return;

        default:
            QObject::customEvent(event);
            return;
        }
    }

    SceneObject* SceneObject::parentObject() const
    {
        return qobject_cast<SceneObject*>(parent());
    }

    bool SceneObject::isRoot() const
    {
        return parentObject() == nullptr;
    }

    HierarchyState& SceneObject::hierarchy()
    {
        return *m_pHierarchy;
    }

    const HierarchyState& SceneObject::hierarchy() const
    {
        return *m_pHierarchy;
    }

    void SceneObject::onOwnPositionChanged()
    {
        SpatialConfigurationChange event(SpatialConfigurationChange::PositionChange);
        QCoreApplication::sendEvent(this, &event);
    }

    void SceneObject::onOwnRotationChanged()
    {
        SpatialConfigurationChange event(SpatialConfigurationChange::RotationChange);
        QCoreApplication::sendEvent(this, &event);
    }

    void SceneObject::onOwnScaleChanged()
    {
        SpatialConfigurationChange event(SpatialConfigurationChange::ScaleChange);
        QCoreApplication::sendEvent(this, &event);
    }

    void SceneObject::handleSpatialConfigurationChange(SpatialConfigurationChange *event)
    {

    }
}
