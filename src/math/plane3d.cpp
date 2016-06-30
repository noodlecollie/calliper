#include "plane3d.h"
#include "callipermath.h"

Plane3D::Plane3D() : m_vecNormal(), m_flDistance(0)
{
}

Plane3D::Plane3D(const QVector3D &normal, float distance) :
    m_vecNormal(), m_flDistance(distance)
{
    setNormal(normal);
}

QVector3D Plane3D::normal() const
{
    return m_vecNormal;
}

bool Plane3D::isNull() const
{
    return m_vecNormal.isNull();
}

void Plane3D::setNormal(const QVector3D &normal)
{
    m_vecNormal = normal.isNull() ? normal : normal.normalized();
}

float Plane3D::distance() const
{
    return m_flDistance;
}

void Plane3D::setDistance(float dist)
{
    m_flDistance = dist;
}

bool Plane3D::operator == (const Plane3D &other) const
{
    return m_vecNormal == other.m_vecNormal &&
            m_flDistance == other.m_flDistance;
}

bool Plane3D::operator != (const Plane3D &other) const
{
    return !(*this == other);
}

bool Plane3D::normalEquals(const Plane3D &other) const
{
    return m_vecNormal == other.m_vecNormal;
}

bool Plane3D::fuzzyNormalEquals(const Plane3D &other) const
{
    return Math::fuzzyVectorEquals(m_vecNormal, other.m_vecNormal);
}

bool Plane3D::distanceEquals(const Plane3D &other) const
{
    return m_flDistance == other.m_flDistance;
}

bool Plane3D::fuzzyDistanceEquals(const Plane3D &other) const
{
    return (qFuzzyIsNull(m_flDistance) && qFuzzyIsNull(other.m_flDistance)) ||
            qFuzzyCompare(m_flDistance, other.m_flDistance);
}

bool Plane3D::isParallelTo(const Plane3D &other) const
{
    return qAbs(QVector3D::dotProduct(m_vecNormal, other.m_vecNormal)) == 1.0f;
}

bool Plane3D::fuzzyIsParallelTo(const Plane3D &other) const
{
    return qFuzzyCompare(qAbs(QVector3D::dotProduct(m_vecNormal, other.m_vecNormal)), 1.0f);
}

QVector3D Plane3D::origin() const
{
    return m_flDistance * m_vecNormal;
}
