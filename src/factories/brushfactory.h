#ifndef BRUSHFACTORY_H
#define BRUSHFACTORY_H

#include "boundingbox.h"
#include "brush.h"

namespace BrushFactory
{
    Brush* fromBoundingBox(BaseScene* scene, SceneObject* parent, const BoundingBox &bbox, const QString &texture, const QVector3D origin = QVector3D(0,0,0));
}

#endif // BRUSHFACTORY_H
