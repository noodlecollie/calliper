#ifndef MODELMATH_H
#define MODELMATH_H

#include "model_global.h"
#include <QMatrix4x4>
#include "eulerangle.h"

namespace NS_MODEL
{
    namespace ModelMath
    {
        QMatrix4x4 matrixOrientation(const EulerAngle &angle);
    }
}

#endif // MODELMATH_H
