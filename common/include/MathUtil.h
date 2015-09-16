#ifndef MATHUTIL_H
#define MATHUTIL_H

#include "PolyVector2.h"
#include "PolyVector3.h"
#include "PolyVector4.h"
#include "PolyMatrix4.h"

using namespace Polycode;
typedef Vector3 CEulerAngle;

// Changing co-ordinate systems:
// Hammer defines X = right, Y = forward, Z = up;
// OpenGL defines X = right, Y = up, Z - backward.
// These matrices convert vectors between the two spaces.
static const Matrix4 HAMMER_TO_OPENGL(1,0, 0,0,
									  0,0,-1,0,
									  0,1, 0,0,
									  0,0, 0,1);

static const Matrix4 OPENGL_TO_HAMMER(1, 0,0,0,
									  0, 0,1,0,
									  0,-1,0,0,
									  0, 0,0,1);

static const Number NUM_COMPARE_EPSILON = sizeof(Number) == sizeof(double) ? 1e-8 : 1e-4;
#define FUZZY_COMPARE(x,y) ((x-y) < NUM_COMPARE_EPSILON)
#define FUZZY_NULL(x) (x < NUM_COMPARE_EPSILON)

bool vectorIsNull(const Vector2 &vec);
bool vectorIsNull(const Vector3 &vec);
bool vectorIsNull(const Vector4 &vec);

bool vectorFuzzyCompare(const Vector2 &v1, const Vector2 &v2);
bool vectorFuzzyCompare(const Vector3 &v1, const Vector3 &v2);
bool vectorFuzzyCompare(const Vector4 &v1, const Vector4 &v2);

// Done in Hammer co-ordinate space.
CEulerAngle vectorToAngle(const Vector3 &vec);
Vector3 angleToVector(const CEulerAngle &angle);
CEulerAngle normaliseAngle(const CEulerAngle &angle);
CEulerAngle clampAngle(const CEulerAngle &angle);

#endif	// MATHUTIL_H
