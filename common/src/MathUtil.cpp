#include "MathUtil.h"

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
	Number radYaw = TORADIANS * angle.yaw;
	Number radPitch = TORADIANS * angle.pitch;
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