#ifndef GEOMETRYFACTORY_H
#define GEOMETRYFACTORY_H

#include <QColor>
#include <QVector3D>
#include "boundingbox.h"
#include <QMatrix4x4>

class GeometryData;

namespace GeometryFactory
{
    // Shader overrides are not set by any of these functions.
    GeometryData* cube(float radius);
    GeometryData* cubeSolidColor(float radius, const QColor &col, bool nullNormals = false);
    GeometryData* cubeFullAtts(float radius);
    GeometryData* triangleQuad(float radius);
    GeometryData* lineRect(float radius, const QColor &col);
    GeometryData* lineCuboid(const QVector3D &min, const QVector3D &max, const QColor &col);
    GeometryData* lineCuboid(const BoundingBox &bbox, const QColor &col);
    GeometryData* cuboidSolidColor(const QVector3D &min, const QVector3D &max, const QColor &col, bool nullNormals = false);
    GeometryData* cuboidSolidColor(const BoundingBox &bbox, const QColor &col, bool nullNormals = false);

    // Length is from base to tip.
    // Width is width of arrow head, which is twice the width of the shaft.
    // Head proportion is the proportion of the length that the head takes up.
    GeometryData* flatArrow(float length, float width, float headProportion, const QColor &col, bool nullNormals = false);

    // Think the move_rope 3D position marker from Hammer.
    GeometryData* hexPin(int subdivisions, float pinRadius);

    GeometryData* fromObjFile(const QString &filename, float scale = 1.0f);
}

#endif // GEOMETRYFACTORY_H
