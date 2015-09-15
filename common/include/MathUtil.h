#ifndef MATHUTIL_H
#define MATHUTIL_H

#include "PolyVector2.h"
#include "PolyVector3.h"
#include "PolyVector4.h"

using namespace Polycode;
typedef Vector3 CEulerAngle;

static const Number NUM_COMPARE_EPSILON = sizeof(Number) == sizeof(double) ? 1e-8 : 1e-4;
#define FUZZY_COMPARE(x,y) ((x-y) < NUM_COMPARE_EPSILON)
#define FUZZY_NULL(x) (x < NUM_COMPARE_EPSILON)

bool vectorIsNull(const Vector2 &vec);
bool vectorIsNull(const Vector3 &vec);
bool vectorIsNull(const Vector4 &vec);

// Done in Hammer co-ordinate space.
CEulerAngle vectorToAngle(const Vector3 &vec);
Vector3 angleToVector(const CEulerAngle &angle);
CEulerAngle normaliseAngle(const CEulerAngle &angle);
CEulerAngle clampAngle(const CEulerAngle &angle);

#endif	// MATHUTIL_H
