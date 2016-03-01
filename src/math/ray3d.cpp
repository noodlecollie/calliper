#include "ray3d.h"
#include <QtMath>

Ray3D::Ray3D()
{
    // Vectors initialised to null.
}

Ray3D::Ray3D(const QVector3D &origin, const QVector3D &direction)
{
    m_vecOrigin = origin;
    m_vecDirection = direction;
}

QVector3D Ray3D::origin() const
{
    return m_vecOrigin;
}

void Ray3D::setOrigin(const QVector3D &origin)
{
    m_vecOrigin = origin;
}

QVector3D Ray3D::direction() const
{
    return m_vecDirection;
}

void Ray3D::setDirection(const QVector3D &direction)
{
    m_vecDirection = direction.isNull() ? direction : direction.normalized();
}

QVector3D Ray3D::parameterise(float t) const
{
    return m_vecOrigin + (t * m_vecDirection);
}

QVector3D Ray3D::parameterise(Math::AxisIdentifier axis, float value, bool *success) const
{
    // We don't want a null ray, but if the ray is null then this check will catch it anyway.
    if ( qFuzzyIsNull(m_vecDirection[axis]) )
    {
        if ( success )
            *success = false;
        return QVector3D();
    }

    if ( success )
        *success = true;

    return parameterise((value - m_vecOrigin[axis])/m_vecDirection[axis]);

}

bool Ray3D::operator ==(const Ray3D &other) const
{
    return m_vecDirection == other.m_vecDirection &&
            m_vecOrigin == other.m_vecOrigin;
}

bool Ray3D::operator !=(const Ray3D &other) const
{
    return !(*this == other);
}

bool Ray3D::directionEquals(const Ray3D &other) const
{
    return m_vecDirection == other.m_vecDirection;
}

bool Ray3D::fuzzyDirectionEquals(const Ray3D &other) const
{
    return Math::fuzzyVectorEquals(m_vecDirection, other.m_vecDirection);
}

bool Ray3D::originEquals(const Ray3D &other) const
{
    return m_vecOrigin == other.m_vecOrigin;
}

bool Ray3D::fuzzyOriginEquals(const Ray3D &other) const
{
    return Math::fuzzyVectorEquals(m_vecOrigin, other.m_vecOrigin);
}

bool Ray3D::isParallelTo(const Ray3D &other) const
{
    return qAbs(QVector3D::dotProduct(m_vecDirection, other.m_vecDirection)) == 1.0f;
}

bool Ray3D::fuzzyIsParallelTo(const Ray3D &other) const
{
    return qFuzzyCompare(qAbs(QVector3D::dotProduct(m_vecDirection, other.m_vecDirection)), 1.0f);
}

bool Ray3D::isEquivalentTo(const Ray3D &other) const
{
    // We are equivalent if:
    // - Both us and the other ray are null.
    // - Neither ray is null, our direction is parallel to the other ray's direction, and the other
    //   ray's origin lies on our line.
    // No ray is equivalent to a null ray unless it itself is null.

    if ( isNull() != other.isNull() )
        return false;
    else
        return isNull() ||
               (
                    isParallelTo(other) &&
                    (
                        originEquals(other) ||
                        qAbs(QVector3D::dotProduct((m_vecOrigin - other.m_vecOrigin).normalized(), m_vecDirection)) == 1.0f
                    )
               );
}

bool Ray3D::fuzzyIsEquivalentTo(const Ray3D &other) const
{
    if ( fuzzyIsNull() != other.fuzzyIsNull() )
        return false;
    else
        return fuzzyIsNull() ||
                (
                    fuzzyIsParallelTo(other) &&
                    (
                        fuzzyOriginEquals(other) ||
                        qFuzzyCompare(qAbs(QVector3D::dotProduct((m_vecOrigin - other.m_vecOrigin).normalized(),
                                                                 m_vecDirection)), 1.0f)
                    )
                );
}

float Ray3D::distanceFrom(const QVector3D &point, bool* success) const
{
    if ( isNull() )
    {
        if ( success )
            *success = false;
        return 0;
    }

    if ( success )
        *success = true;

    // This would usually be divided by the length of the directional vector, but since this is
    // a unit vector we don't need to do that.
    return QVector3D::crossProduct(m_vecOrigin - point, m_vecDirection).length();
}

bool Ray3D::isNull() const
{
    return m_vecDirection.isNull();
}

bool Ray3D::fuzzyIsNull() const
{
    return Math::fuzzyVectorIsNull(m_vecDirection);
}
