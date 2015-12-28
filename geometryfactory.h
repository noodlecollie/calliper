#ifndef GEOMETRYFACTORY_H
#define GEOMETRYFACTORY_H

class GeometryData;

namespace GeometryFactory
{
    GeometryData* cube(float radius);
    GeometryData* fullScreenQuad();
}

#endif // GEOMETRYFACTORY_H
