#ifndef RAY3D_H
#define RAY3D_H

#include <QVector3D>
#include "callipermath.h"

class Ray3D
{
public:
    // Constructs a null ray (a ray with a null direction).
    Ray3D();
    Ray3D(const QVector3D &origin, const QVector3D &direction);

    // A null ray has a null direction.
    bool isNull() const;
    bool fuzzyIsNull() const;

    QVector3D origin() const;
    void setOrigin(const QVector3D &origin);

    QVector3D direction() const;
    void setDirection(const QVector3D &direction);

    // Returns ray where axis=value (ie. ray intersection with plane of this type).
    QVector3D parameterise(Math::AxisIdentifier axis, float value, bool* success = NULL) const;
    QVector3D parameterise(float t) const;
    float distanceFrom(const QVector3D &point, bool* success = NULL) const;

    bool operator ==(const Ray3D &other) const;
    bool operator !=(const Ray3D &other) const;

    bool directionEquals(const Ray3D &other) const;
    bool fuzzyDirectionEquals(const Ray3D &other) const;

    bool originEquals(const Ray3D &other) const;
    bool fuzzyOriginEquals(const Ray3D &other) const;

    bool isParallelTo(const Ray3D &other) const;
    bool fuzzyIsParallelTo(const Ray3D &other) const;

    bool isEquivalentTo(const Ray3D &other) const;
    bool fuzzyIsEquivalentTo(const Ray3D &other) const;

private:
    QVector3D m_vecOrigin;
    QVector3D m_vecDirection;
};

#endif // RAY3D_H
