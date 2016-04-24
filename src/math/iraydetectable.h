#ifndef IRAYDETECTABLE_H
#define IRAYDETECTABLE_H

#include "ray3d.h"
#include <QColor>

class IRayDetectable
{
public:
    virtual ~IRayDetectable() {}

    // Assumes that the ray is in local space.
    // Returns qInf() if no intersection occurred.
    virtual float computeIntersection(const Ray3D &ray, QRgb* col = NULL) const = 0;
};

#endif // IRAYDETECTABLE_H

