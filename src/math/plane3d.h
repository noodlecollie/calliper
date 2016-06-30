#ifndef PLANE3D_H
#define PLANE3D_H

#include <QVector3D>

class Plane3D
{
public:
    // Constructs a null plane with no normal.
    Plane3D();
    Plane3D(const QVector3D &normal, float distance);

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

private:
    QVector3D   m_vecNormal;
    float       m_flDistance;
};

#endif // PLANE3D_H
