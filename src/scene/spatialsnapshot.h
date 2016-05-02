#ifndef SPATIALSNAPSHOT_H
#define SPATIALSNAPSHOT_H

#include <QVector3D>
#include "eulerangle.h"
#include <QMatrix4x4>
#include "callipermath.h"
#include <QtDebug>

class SpatialSnapshot
{
public:
    SpatialSnapshot(const QVector3D &trn = QVector3D(), const EulerAngle &rot = EulerAngle(),
                    const QVector3D &scl = QVector3D(1,1,1)) :
        translation(trn),
        rotation(rot),
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
        return SpatialSnapshot((mat * QVector4D(translation, 1)).toVector3D(),
                               Math::vectorToAngleSimple((mat * QVector4D(Math::angleToVectorSimple(rotation), 0)).toVector3D()),
                               (mat * QVector4D(scale, 0).toVector3D()));
    }

    QVector3D   translation;
    EulerAngle  rotation;
    QVector3D   scale;
};

QDebug operator <<(QDebug& debug, const SpatialSnapshot &sshot);

#endif // SPATIALSNAPSHOT_H
