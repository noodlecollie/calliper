#ifndef GEOMETRYFACTORY_H
#define GEOMETRYFACTORY_H

#include <QColor>

class GeometryData;

namespace GeometryFactory
{
    GeometryData* cube(float radius);
    GeometryData* triangleQuad(float radius);
    GeometryData* lineRect(float radius, const QColor &col);
}

#endif // GEOMETRYFACTORY_H
