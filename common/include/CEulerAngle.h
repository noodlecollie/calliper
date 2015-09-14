#ifndef CEULER_ANGLE_H
#define CEULER_ANGLE_H

#include "PolyGlobals.h"

class CEulerAngle
{
public:
	CEulerAngle(Number pitch, Number yaw, Number roll);
	CEulerAngle();
	
	// Normalises the angle to be represented by values [-90 90] [0 360) [-180 180].
	void normalise();
	CEulerAngle normalised() const;
	
	// Clamps the angle to be represented by values [-90 90] [0 360) [-180 180].
	void clamp();
	CEulerAngle clamped() const;
	
	inline bool operator ==(const CEulerAngle &other)
	{
		return pitch == other.pitch &&
		yaw == other.yaw &&
		roll == other.roll;
	}
	
	inline bool operator !=(const CEulerAngle &other)
	{
		return !(*this == other);
	}
	
public:
	Number pitch;
	Number yaw;
	Number roll;
	
private:
	static void makeNormalised(Number &p, Number &y, Number &r);
	static void performClamp(Number &p, Number &y, Number &r);
};

#endif	// CEULER_ANGLE_H
