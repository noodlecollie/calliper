#include "modelmath.h"
#include "math/math.h"
#include <QtMath>

namespace NS_MODEL
{
    namespace ModelMath
    {
        QMatrix4x4 matrixOrientation(const EulerAngle &angle)
        {
            return NS_CALLIPERUTIL::Math::matrixRotateZ(qDegreesToRadians(angle.yaw()))
                    * NS_CALLIPERUTIL::Math::matrixRotateY(qDegreesToRadians(angle.pitch()))
                    * NS_CALLIPERUTIL::Math::matrixRotateX(qDegreesToRadians(angle.roll()));
        }
    }
}
