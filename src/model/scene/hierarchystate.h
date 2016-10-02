#ifndef HIERARCHYSTATE_H
#define HIERARCHYSTATE_H

#include "model_global.h"
#include <QObject>
#include <QVector3D>
#include "math/eulerangle.h"

namespace NS_MODEL
{
    class MODELSHARED_EXPORT HierarchyState : public QObject
    {
        Q_OBJECT
    public:
        explicit HierarchyState(QObject* parent = 0);

        QVector3D position() const;
        void setPosition(const QVector3D &pos);

        EulerAngle rotation() const;
        void setRotation(const EulerAngle &rot);

        QVector3D scale() const;
        void setScale(const QVector3D &scl);
        void setScale(float scl);

    signals:
        void positionChanged();
        void rotationChanged();
        void scaleChanged();

    private:
        QVector3D   m_vecPosition;
        EulerAngle  m_angRotation;
        QVector3D   m_vecScale;
    };
}

#endif // HIERARCHYSTATE_H
