#ifndef SPATIALSNAPSHOT_H
#define SPATIALSNAPSHOT_H

#include <QVector3D>
#include "eulerangle.h"
#include <QMatrix4x4>
#include "callipermath.h"

struct SpatialSnapshot
{
public:
    SpatialSnapshot(const QVector3D &pos = QVector3D(), const EulerAngle &ang = EulerAngle(),
                    const QVector3D &scl = QVector3D(1,1,1)) :
        position(pos),
        angles(ang),
        scale(scl)
    {
    }

    SpatialSnapshot& transform(const QMatrix4x4 &mat)
    {
        *this = transformed(mat);
        return *this;
    }

    SpatialSnapshot transformed(const QMatrix4x4 &mat) const
    {
        return SpatialSnapshot((mat * QVector4D(position, 1)).toVector3D(),
                               Math::vectorToAngleSimple((mat * QVector4D(Math::angleToVectorSimple(angles), 0)).toVector3D()),
                               (mat * QVector4D(scale, 0).toVector3D()));
    }

    QVector3D   position;
    EulerAngle  angles;
    QVector3D   scale;
};

#endif // SPATIALSNAPSHOT_H
