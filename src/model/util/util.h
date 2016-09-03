#ifndef UTIL_H
#define UTIL_H

#include "model_global.h"

namespace NS_MODEL
{
    namespace Util
    {
        // Format (x y z)
        QVector3D vectorFromVmfCoord(const QString &coord);

        // Format (x y z) (x y z) (x y z)
        void vectorsFromVmfCoords(const QString &coords, QVector3D &v0, QVector3D &v1, QVector3D &v2);
    }
}

#endif // UTIL_H
