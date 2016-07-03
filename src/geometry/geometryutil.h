#ifndef GEOMETRYUTIL_H
#define GEOMETRYUTIL_H

#include <QList>
#include <QVector3D>
#include "winding3d.h"

namespace GeometryUtil
{
    QList<QVector3D> windingsToVertices(QList<Winding3D*> &windings);
}

#endif // GEOMETRYUTIL_H
