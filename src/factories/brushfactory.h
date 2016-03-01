#ifndef BRUSHFACTORY_H
#define BRUSHFACTORY_H

#include "boundingbox.h"
#include "brush.h"

namespace BrushFactory
{
    Brush* fromBoundingBox(const BoundingBox &bbox, SceneObject* scene, const QString &texture);
}

#endif // BRUSHFACTORY_H
