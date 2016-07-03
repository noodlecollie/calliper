#ifndef BRUSHFACTORY_H
#define BRUSHFACTORY_H

#include "boundingbox.h"
#include "brush.h"
#include <QList>
#include "texturedpolygon.h"

namespace BrushFactory
{
    Brush* fromBoundingBox(BaseScene* scene, SceneObject* parent, const BoundingBox &bbox, const QString &texture, const QVector3D origin = QVector3D(0,0,0));
    Brush* fromPolygons(BaseScene* scene, SceneObject* parent, const QList<TexturedPolygon> &polygons);
}

#endif // BRUSHFACTORY_H
