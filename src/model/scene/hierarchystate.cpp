#include "hierarchystate.h"

namespace NS_MODEL
{
    HierarchyState::HierarchyState(QObject* parent)
        : QObject(parent),
          m_vecPosition(),
          m_angRotation(),
          m_vecScale(1,1,1)
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
        emit rotationChanged();
    }

    QVector3D HierarchyState::scale() const
    {
        return m_vecScale;
    }

    void HierarchyState::setScale(const QVector3D &scl)
    {
        if ( scl == m_vecScale )
            return;

        m_vecScale = scl;
        emit scaleChanged();
    }

    void HierarchyState::setScale(float scl)
    {
        setScale(QVector3D(scl, scl, scl));
    }
}
