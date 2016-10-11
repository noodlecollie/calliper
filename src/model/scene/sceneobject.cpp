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

        m_pHierarchy->cloneFrom(cloneFrom->hierarchy());
        m_bNeedsRendererUpdate = cloneFrom->m_bNeedsRendererUpdate;
    }

    SceneObject::~SceneObject()
    {

    }

    void SceneObject::commonInit()
    {
        m_pHierarchy = initHierarchyState(true);
        m_bNeedsRendererUpdate = true;
    }

    HierarchyState* SceneObject::initHierarchyState(bool isScalable)
    {
        HierarchyState* hs = new HierarchyState(isScalable, this);
        connect(hs, &HierarchyState::positionChanged, this, &SceneObject::onOwnPositionChanged);
        connect(hs, &HierarchyState::rotationChanged, this, &SceneObject::onOwnRotationChanged);
        connect(hs, &HierarchyState::scaleChanged, this, &SceneObject::onOwnScaleChanged);

        return hs;
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
        Q_UNUSED(event);
        flagNeedsRendererUpdate();
    }

    bool SceneObject::needsRendererUpdate() const
    {
        return m_bNeedsRendererUpdate;
    }

    void SceneObject::flagNeedsRendererUpdate()
    {
        m_bNeedsRendererUpdate = true;
    }

    void SceneObject::rendererUpdate(NS_RENDERER::GeometryBuilder &builder) const
    {
        // Call virtual function so that subclasses build their own geometry.
        bakeGeometry(builder);
        m_bNeedsRendererUpdate = false;
    }

    void SceneObject::bakeGeometry(NS_RENDERER::GeometryBuilder &builder) const
    {
        Q_UNUSED(builder);
    }

    bool SceneObject::scalable() const
    {
        return true;
    }

    void SceneObject::updateScalableState(bool isScalable)
    {
        HierarchyState* hs = initHierarchyState(isScalable);
        hs->cloneFrom(*m_pHierarchy);
        delete m_pHierarchy;
        m_pHierarchy = hs;
    }
}
