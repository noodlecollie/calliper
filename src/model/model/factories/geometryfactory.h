#ifndef GEOMETRYFACTORY_H
#define GEOMETRYFACTORY_H

#include "model_global.h"
#include "renderer/geometry/geometrybuilder.h"
#include <QColor>

namespace Model
{
    namespace GeometryFactory
    {
        MODELSHARED_EXPORT void cube(Renderer::GeometryBuilder &builder, float radius, const QColor &color);
        MODELSHARED_EXPORT void wireframeCube(Renderer::GeometryBuilder &builder, float radius, const QColor &color);
    }
}

#endif // GEOMETRYFACTORY_H
