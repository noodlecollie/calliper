#include "CEulerAngle.h"
#include <tgmath.h>

CEulerAngle::CEulerAngle()
{
	pitch = 0;
	yaw = 0;
	roll = 0;
}

CEulerAngle::CEulerAngle(Number pitch, Number yaw, Number roll)
{
	this->pitch = pitch;
	this->yaw = yaw;
	this->roll = roll;
}

void CEulerAngle::normalise()
{
	makeNormalised(pitch, yaw, roll);
}

void CEulerAngle::makeNormalised(Number &p, Number &y, Number &r)
{
	// First normalise pitch.
	// If the pitch is facing the wrong way (ie. opposite to the yaw), flip the yaw and roll.
	Number pitchMod = fmod(p + 90.0f, 360.0f);
	if ( pitchMod > 180.0f )
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

CEulerAngle CEulerAngle::normalised() const
{
	Number p = pitch, y = yaw, r = roll;
	makeNormalised(p,y,r);
	
	// Return a new angle.
	return CEulerAngle(p,y,r);
}

void CEulerAngle::performClamp(Number &p, Number &y, Number &r)
{
	// Clamp pitch [-90 90]
	if ( p < -90.0f ) p = -90.0f;
	else if ( p > 90.0f ) p = 90.0f;
	
	// Clamp roll [-180 180]
	if ( r < -180.0f ) r = -180.0f;
	else if ( r > 180.0f ) r = 180.0f;
	
	// Normalise yaw.
	y = fmod(y, 360.0f);
}

void CEulerAngle::clamp()
{
	performClamp(pitch, yaw, roll);
}

CEulerAngle CEulerAngle::clamped() const
{
	Number p = pitch, y = yaw, r = roll;
	performClamp(p, y, r);
	
	// Return a new angle.
	return CEulerAngle(p,y,r);
}