#ifndef PLANE3D_H
#define PLANE3D_H

#include "model_global.h"
#include <QVector3D>
#include <QtDebug>
#include <QObject>

namespace NS_MODEL
{
    class Plane3D
    {
        Q_GADGET
    public:
        enum PointLocation
        {
            BehindPlane = -1,
            OnPlane = 0,
            InFrontOfPlane = 1,
        };
        Q_ENUMS(PointLocation)

        // Constructs a null plane with no normal.
        Plane3D();
        Plane3D(const QVector3D &normal, float distance);
        Plane3D(const QVector3D &v0, const QVector3D &v1, const QVector3D &v2);

        // A null plane has no normal.
        bool isNull() const;

        QVector3D normal() const;
        void setNormal(const QVector3D &normal);

        float distance() const;
        void setDistance(float dist);

        bool operator == (const Plane3D &other) const;
        bool operator != (const Plane3D &other) const;

        bool normalEquals(const Plane3D &other) const;
        bool fuzzyNormalEquals(const Plane3D &other) const;

        bool distanceEquals(const Plane3D &other) const;
        bool fuzzyDistanceEquals(const Plane3D &other) const;

        bool isParallelTo(const Plane3D &other) const;
        bool fuzzyIsParallelTo(const Plane3D &other) const;

        QVector3D origin() const;

        PointLocation getPointLocation(const QVector3D &point) const;

    private:
        QVector3D   m_vecNormal;
        float       m_flDistance;
    };

    QDebug operator <<(QDebug debug, const Plane3D &plane);
}

#endif // PLANE3D_H
