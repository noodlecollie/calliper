#include "hierarchyparams.h"

namespace NS_MODEL
{
    HierarchyParams::HierarchyParams(QObject* parent)
        : QObject(parent),
          m_vecPosition(),
          m_angRotation(),
          m_vecScale(1,1,1)
    {

    }

    QVector3D HierarchyParams::position() const
    {
        return m_vecPosition;
    }

    void HierarchyParams::setPosition(const QVector3D &pos)
    {
        if ( pos == m_vecPosition )
            return;

        m_vecPosition = pos;
        emit positionChanged();
    }

    EulerAngle HierarchyParams::rotation() const
    {
        return m_angRotation;
    }

    void HierarchyParams::setRotation(const EulerAngle &rot)
    {
        if ( rot == m_angRotation )
            return;

        m_angRotation = rotation;
        emit rotationChanged();
    }

    QVector3D HierarchyParams::scale() const
    {
        return m_vecScale;
    }

    void HierarchyParams::setScale(const QVector3D &scl)
    {
        if ( scl == m_vecScale )
            return;

        m_vecScale = scl;
        emit scaleChanged();
    }

    void HierarchyParams::setScale(float scl)
    {
        setScale(QVector3D(scl, scl, scl));
    }
}
