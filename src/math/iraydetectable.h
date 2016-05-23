#ifndef IRAYDETECTABLE_H
#define IRAYDETECTABLE_H

#include "ray3d.h"
#include <QColor>
#include "raytracecontact.h"

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
    virtual bool computeIntersection(const Ray3D &ray, RayTraceContact &contact, RayCoordinateSpace space = WorldSpace) const = 0;
};

#endif // IRAYDETECTABLE_H

