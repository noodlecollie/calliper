#ifndef IRAYDETECTABLE_H
#define IRAYDETECTABLE_H

#include "ray3d.h"
#include <QColor>

class IRayDetectable
{
public:
    virtual ~IRayDetectable() {}

    enum RayCoordinateSpace
    {
        WorldSpace = 0,
        ParentSpace,
        LocalSpace
    };

    // Assumes that the ray is in local space.
    // Returns qInf() if no intersection occurred.
    virtual float computeIntersection(const Ray3D &ray, QRgb* col = NULL, RayCoordinateSpace space = WorldSpace) const = 0;
};

#endif // IRAYDETECTABLE_H

