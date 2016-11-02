#ifndef RAY3D_H
#define RAY3D_H

#include "model_global.h"
#include <QVector3D>
#include "math/math.h"

namespace NS_MODEL
{
    class Plane3D;

    class Ray3D
    {
    public:
        enum IntersectionType
        {
            NoIntersection = 0,
            SingleIntersection,
            MultipleIntersections,
        };

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
        QVector3D parameterise(CalliperUtil::Math::AxisIdentifier axis, float value, IntersectionType* intersection = nullptr) const;
        QVector3D parameterise(float t) const;
        QVector3D parameterise(const Plane3D &plane, IntersectionType* intersection = nullptr) const;
        float distanceFrom(const QVector3D &point, bool* success = nullptr) const;
        QVector3D atDistanceFromOrigin(float distance) const;

        Ray3D& transform(const QMatrix4x4 &mat);
        Ray3D transformed(const QMatrix4x4 &mat) const;

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

        inline static bool inFrontOfOrigin(float t)
        {
            return t > 0.0f && t < (float)qInf();
        }

    private:
        QVector3D m_vecOrigin;
        QVector3D m_vecDirection;
    };

    QDebug operator <<(QDebug debug, const Ray3D &ray);
}

#endif // RAY3D_H
