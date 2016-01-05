#ifndef GEOMETRYFACTORY_H
#define GEOMETRYFACTORY_H

#include <QColor>
#include <QVector3D>
#include "boundingbox.h"

class GeometryData;

namespace GeometryFactory
{
    // Shader overrides are not set by any of these functions.
    GeometryData* cube(float radius);
    GeometryData* triangleQuad(float radius);
    GeometryData* lineRect(float radius, const QColor &col);
    GeometryData* lineCuboid(const QVector3D &min, const QVector3D &max, const QColor &col);
    GeometryData* lineCuboid(const BoundingBox &bbox, const QColor &col);

    GeometryData* translationHandle(const QColor &col);
}

#endif // GEOMETRYFACTORY_H
