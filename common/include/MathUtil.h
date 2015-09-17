#ifndef MATHUTIL_H
#define MATHUTIL_H

#include "PolyVector2.h"
#include "PolyVector3.h"
#include "PolyVector4.h"
#include "PolyMatrix4.h"

using namespace Polycode;

class CEulerAngle;
class CVector3;

static const Number NUM_COMPARE_EPSILON = sizeof(Number) == sizeof(double) ? 1e-8 : 1e-4;
#define FUZZY_COMPARE(x,y) ((x-y) < NUM_COMPARE_EPSILON)
#define FUZZY_NULL(x) (x < NUM_COMPARE_EPSILON)

bool vectorIsNull(const Vector2 &vec);
bool vectorIsNull(const Vector3 &vec);
bool vectorIsNull(const Vector4 &vec);

bool vectorFuzzyCompare(const Vector2 &v1, const Vector2 &v2);
bool vectorFuzzyCompare(const Vector3 &v1, const Vector3 &v2);
bool vectorFuzzyCompare(const Vector4 &v1, const Vector4 &v2);

CEulerAngle normaliseAngle(const CEulerAngle &angle);
CEulerAngle clampAngle(const CEulerAngle &angle);

// Done in Hammer co-ordinate space.
namespace HammerCoords
{
	CEulerAngle vectorToAngle(const CVector3 &vec);
	CVector3 angleToVector(const CEulerAngle &angle);
}

void perspectiveMatrix(Matrix4 &mat, Number fov, Number aspect, Number nearDist, Number farDist, Number leftHanded = true);

#endif	// MATHUTIL_H
