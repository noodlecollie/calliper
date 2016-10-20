#ifndef HIERARCHYSTATE_H
#define HIERARCHYSTATE_H

#include "model_global.h"
#include <QObject>
#include <QVector3D>
#include "math/eulerangle.h"
#include <QMatrix4x4>

namespace NS_MODEL
{
    class MODELSHARED_EXPORT HierarchyState : public QObject
    {
        Q_OBJECT
    public:
        explicit HierarchyState(bool scalable = true, QObject* parent = 0);

        QVector3D position() const;
        void setPosition(const QVector3D &pos);

        EulerAngle rotation() const;
        void setRotation(const EulerAngle &rot);

        QVector3D scale() const;
        void setScale(const QVector3D &scl);
        void setScale(float scl);

        // If not scalable, scale is not set.
        void cloneFrom(const HierarchyState& other);

        bool scalable() const;

        QMatrix4x4 parentToLocal() const;
        QMatrix4x4 localToParent() const;

    signals:
        void positionChanged();
        void rotationChanged();
        void scaleChanged();

    private:
        void rebuildMatrices() const;

        QVector3D   m_vecPosition;
        EulerAngle  m_angRotation;
        QVector3D   m_vecScale;
        bool m_bScalable;

        mutable QMatrix4x4 m_matParentToLocal;
        mutable QMatrix4x4 m_matLocalToParent;
        mutable bool m_bMatricesStale;
    };
}

#endif // HIERARCHYSTATE_H
