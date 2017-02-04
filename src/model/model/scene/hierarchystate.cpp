#include "hierarchystate.h"
#include "calliperutil/math/math.h"
#include "model/math/modelmath.h"

namespace Model
{
    HierarchyState::HierarchyState(bool scalable, QObject* parent)
        : QObject(parent),
          m_vecPosition(),
          m_angRotation(),
          m_vecScale(1,1,1),
          m_bScalable(scalable),
          m_matParentToLocal(),
          m_matLocalToParent(),
          m_bMatricesStale(true)
    {

    }

    QVector3D HierarchyState::position() const
    {
        return m_vecPosition;
    }

    void HierarchyState::setPosition(const QVector3D &pos)
    {
        if ( pos == m_vecPosition )
            return;

        m_vecPosition = pos;
        m_bMatricesStale = true;
        emit positionChanged();
    }

    EulerAngle HierarchyState::rotation() const
    {
        return m_angRotation;
    }

    void HierarchyState::setRotation(const EulerAngle &rot)
    {
        if ( rot == m_angRotation )
            return;

        m_angRotation = rot;
        m_bMatricesStale = true;
        emit rotationChanged();
    }

    QVector3D HierarchyState::scale() const
    {
        return m_vecScale;
    }

    void HierarchyState::setScale(const QVector3D &scl)
    {
        if ( scl == m_vecScale || !m_bScalable )
            return;

        m_vecScale = scl;
        m_bMatricesStale = true;
        emit scaleChanged();
    }

    void HierarchyState::setScale(float scl)
    {
        setScale(QVector3D(scl, scl, scl));
    }

    void HierarchyState::cloneFrom(const HierarchyState &other)
    {
        setPosition(other.position());
        setRotation(other.rotation());
        setScale(other.scale());
    }

    QMatrix4x4 HierarchyState::parentToLocal() const
    {
        if ( m_bMatricesStale )
            rebuildMatrices();

        return m_matParentToLocal;
    }

    QMatrix4x4 HierarchyState::localToParent() const
    {
        if ( m_bMatricesStale )
            rebuildMatrices();

        return m_matLocalToParent;
    }

    void HierarchyState::rebuildMatrices() const
    {
        using namespace CalliperUtil;

        m_matLocalToParent = Math::matrixTranslate(m_vecPosition)
                * ModelMath::matrixOrientation(m_angRotation)
                * Math::matrixScale(m_vecScale);

        m_matParentToLocal = m_matLocalToParent.inverted();
        m_bMatricesStale = false;
    }

    bool HierarchyState::scalable() const
    {
        return m_bScalable;
    }
}
