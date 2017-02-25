#include "sceneobject.h"
#include <QCoreApplication>
#include "model/events/modeleventtypes.h"
#include <QtDebug>
#include "calliperutil/math/math.h"
#include "model/math/modelmath.h"

namespace Model
{
    SceneObject::SceneObject(const SceneObjectInitParams &initParams, SceneObject* parentObject)
        : QObject(parentObject), m_pParentScene(initParams.m_pScene), m_iObjectId(initParams.m_iObjectId)
    {
        commonInit();
    }

    SceneObject::SceneObject(const SceneObject* cloneFrom, const SceneObjectInitParams &initParams)
        : QObject(cloneFrom->parentObject()), m_pParentScene(initParams.m_pScene),
          m_iObjectId(initParams.m_iObjectId)
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
        Q_ASSERT_X(m_pParentScene, Q_FUNC_INFO, "Must have a valid parent scene!");
        m_pHierarchy = initHierarchyState(true);
        m_colColor = QColor::fromRgb(0xffffffff);
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

    void SceneObject::rendererUpdate(const ShaderPalette &shaderPalette, Renderer::GeometryBuilder &builder) const
    {
        // Call virtual function so that subclasses build their own geometry.
        bakeGeometry(shaderPalette, builder);

        if ( !customVertexColours() )
        {
            updateGeometryColours(builder);
        }

        m_bNeedsRendererUpdate = false;
    }

    void SceneObject::bakeGeometry(const ShaderPalette &shaderPalette, Renderer::GeometryBuilder &builder) const
    {
        Q_UNUSED(shaderPalette);
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

    QMatrix4x4 SceneObject::localToRootMatrix() const
    {
        QMatrix4x4 mat;
        const SceneObject* obj = this;

        do
        {
            // We premultiply each matrix to go from the current object to its parent.
            mat = obj->hierarchy().localToParent() * mat;
            obj = obj->parentObject();
        }
        while ( obj );

        return mat;
    }

    QMatrix4x4 SceneObject::rootToLocalMatrix() const
    {
        return localToRootMatrix().inverted();
    }

    QList<SceneObject*> SceneObject::childSceneObjects() const
    {
        return findChildren<SceneObject*>(QString(), Qt::FindDirectChildrenOnly);
    }

    Scene* SceneObject::parentScene() const
    {
        return m_pParentScene;
    }

    quint32 SceneObject::objectId() const
    {
        return m_iObjectId;
    }

    bool SceneObject::customVertexColours() const
    {
        return false;
    }

    void SceneObject::updateGeometryColours(Renderer::GeometryBuilder &builder) const
    {
        using namespace Renderer;

        for ( int i = 0; i < builder.sectionCount(); i++ )
        {
            updateGeometryColours(builder.section(i));
        }
    }

    void SceneObject::updateGeometryColours(Renderer::GeometrySection *section) const
    {
        using namespace Renderer;

        section->clearAttribute(GeometrySection::ColorAttribute);
        if ( section->vertexFormat().colorComponents() < 1 )
            return;

        while ( section->attributeCount(GeometrySection::ColorAttribute) < section->attributeCount(GeometrySection::PositionAttribute) )
        {
            section->addColor(m_colColor);
        }
    }

    QColor SceneObject::color() const
    {
        return m_colColor;
    }

    void SceneObject::setColor(const QColor &col)
    {
        if ( col == m_colColor )
            return;

        m_colColor = col;
        flagNeedsRendererUpdate();
    }

    bool SceneObject::mustExist() const
    {
        return m_bMustExist;
    }

    void SceneObject::setMustExist(bool mustExist)
    {
        m_bMustExist = mustExist;
    }

    void SceneObject::setParentObject(SceneObject *newParent)
    {
        if ( newParent == this )
        {
            return;
        }

        Q_ASSERT_X(newParent->parentScene() == parentScene(), Q_FUNC_INFO, "Cannot set parent to an object in a different scene!");
        if ( newParent->parentScene() != parentScene() )
        {
            return;
        }

        SceneObject* oldParent = parentObject();
        Q_ASSERT(oldParent);

        QMatrix4x4 oldParentToNewParent = newParent->rootToLocalMatrix() * oldParent->localToRootMatrix();

        QVector3D newPosition = CalliperUtil::Math::transformVectorPosition(hierarchy().position(), oldParentToNewParent);

        QVector3D fwd, right, up;
        ModelMath::angleToVectors(hierarchy().rotation(), fwd, right, up);
        fwd = CalliperUtil::Math::transformVectorDirection(fwd, oldParentToNewParent);
        up = CalliperUtil::Math::transformVectorDirection(up, oldParentToNewParent);
        EulerAngle newAngles = ModelMath::vectorsToAngle(fwd, up);

        QVector3D newScale = CalliperUtil::Math::transformVectorDirection(hierarchy().scale(), oldParentToNewParent);

        setParent(newParent);
        hierarchy().setPosition(newPosition);
        hierarchy().setRotation(newAngles);
        hierarchy().setScale(newScale);
    }
}
