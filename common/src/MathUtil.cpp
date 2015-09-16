#include "MathUtil.h"

#if PLATFORM == PLATFORM_WINDOWS
#include <ctgmath>
#else
#include <tgmath.h>
#endif

bool vectorIsNull(const Vector2 &vec)
{
	return vec.x == 0.0f && vec.y == 0.0f;
}

bool vectorIsNull(const Vector3 &vec)
{
	return vec.x == 0.0f && vec.y == 0.0f && vec.z == 0.0f;
}
bool vectorIsNull(const Vector4 &vec)
{
	return vec.x == 0.0f && vec.y == 0.0f && vec.z == 0.0f && vec.w == 0.0f;
}

Vector3 angleToVector(const CEulerAngle &angle)
{
	// X and Z are dependent on the pitch and yaw; Y is dependent on the pitch only.
	Number radYaw = TORADIANS * angle.y;
	Number radPitch = TORADIANS * angle.x;
	Number sinPitch = sin(radPitch);
	Number sinYaw = sin(radYaw);
	Number cosPitch = cos(radPitch);
	Number cosYaw = cos(radYaw);
	
	return Vector3(cosPitch * cosYaw, cosPitch * sinYaw, -sinPitch);
}

// Method taken from http://www.gamedev.net/topic/399701-convert-vector-to-euler-cardan-angles/#entry3651854
CEulerAngle vectorToAngle(const Vector3 &vec)
{
	// If the vector is null, return a zeroed angle.
	if ( vectorIsNull(vec) )
	{
		return CEulerAngle();
	}
	
	// If x and y are null, just set the pitch.
	if ( FUZZY_NULL(vec.x) && FUZZY_NULL(vec.y) )
	{
		return CEulerAngle(vec.z > 0.0f ? 270.0f : 90.0f, 0.0f, 0.0f);
	}
	
	Number temp, yaw, pitch;
	
	// Yaw depends on the x and y co-ordinates.
	yaw = TODEGREES * atan2(vec.y, vec.x);
	if ( yaw < 0.0f ) yaw += 360.0f;
	
	// Pitch is found by finding the angle between the xy projection of the vector
	// and the negative Z axis.
	temp = sqrt(vec.x * vec.x + vec.y * vec.y);   // Length of projection onto xy plane
	pitch = TODEGREES * atan(-vec.z / temp);      // Angle between this and -z.
	if ( pitch < 0.0f ) pitch += 360;
	
	return CEulerAngle(pitch, yaw, 0.0f);
}

void performClamp(Number &p, Number &y, Number &r)
{
	// Clamp pitch [-90 90]
	if (p < -90.0f) p = -90.0f;
	else if (p > 90.0f) p = 90.0f;

	// Clamp roll [-180 180]
	if (r < -180.0f) r = -180.0f;
	else if (r > 180.0f) r = 180.0f;

	// Normalise yaw.
	y = fmod(y, 360.0f);
}

void makeNormalised(Number &p, Number &y, Number &r)
{
	// First normalise pitch.
	// If the pitch is facing the wrong way (ie. opposite to the yaw), flip the yaw and roll.
	Number pitchMod = fmod(p + 90.0f, 360.0f);
	if (pitchMod > 180.0f)
	{
		y += 180.0f;
		r += 180.0f;

		if (pitchMod <= 270.0f)
			p = -180.0f - p;
		else
			p = 180.0f - p;
	}

	// Normalise pitch value in case it's extraneous.
	// By this point we know it'll end up [-90 90].
	p = fmod(p, 360.0f);

	// Normalise yaw value.
	y = fmod(y, 360.0f);

	// Normalise the roll [-180 180].
	while (r < -180.0f)
		r += 360.0f;
	while (r > 180.0f)
		r -= 360.0f;

}

CEulerAngle normaliseAngle(const CEulerAngle &angle)
{
	Number pitch = angle.x, yaw = angle.y, roll = angle.z;
	makeNormalised(pitch, yaw, roll);
	return CEulerAngle(pitch, yaw, roll);
}

CEulerAngle clampAngle(const CEulerAngle &angle)
{
	Number pitch = angle.x, yaw = angle.y, roll = angle.z;
	performClamp(pitch, yaw, roll);
	return CEulerAngle(pitch, yaw, roll);
}

bool vectorFuzzyCompare(const Vector2 &v1, const Vector2 &v2)
{
	return FUZZY_COMPARE(v1.x, v2.x) && FUZZY_COMPARE(v1.y, v2.y);
}

bool vectorFuzzyCompare(const Vector3 &v1, const Vector3 &v2)
{
	return FUZZY_COMPARE(v1.x, v2.x) && FUZZY_COMPARE(v1.y, v2.y) && FUZZY_COMPARE(v1.z, v2.z);
}

bool vectorFuzzyCompare(const Vector4 &v1, const Vector4 &v2)
{
	return FUZZY_COMPARE(v1.x, v2.x) && FUZZY_COMPARE(v1.y, v2.y)
	&& FUZZY_COMPARE(v1.z, v2.z) && FUZZY_COMPARE(v1.w, v2.w);
}