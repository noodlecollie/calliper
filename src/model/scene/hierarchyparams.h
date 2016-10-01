#ifndef HIERARCHYPARAMS_H
#define HIERARCHYPARAMS_H

#include "model_global.h"
#include <QObject>
#include <QVector3D>
#include "math/eulerangle.h"

namespace NS_MODEL
{
    class HierarchyParams : public QObject
    {
        Q_OBJECT
    public:
        explicit HierarchyParams(QObject* parent = 0);

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

#endif // HIERARCHYPARAMS_H
