#ifndef GEOMETRYFACTORY_H
#define GEOMETRYFACTORY_H

#include "model_global.h"
#include "geometry/geometrybuilder.h"
#include <QColor>

namespace NS_MODEL
{
    namespace GeometryFactory
    {
        MODELSHARED_EXPORT void cube(NS_RENDERER::GeometryBuilder &builder, float radius, const QColor &color);
        MODELSHARED_EXPORT void wireframeCube(NS_RENDERER::GeometryBuilder &builder, float radius, const QColor &color);
    }
}

#endif // GEOMETRYFACTORY_H
