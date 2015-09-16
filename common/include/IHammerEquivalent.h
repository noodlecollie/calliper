#ifndef IHAMMEREQUIVALENT_H
#define IHAMMEREQUIVALENT_H

#include "PolyVector3.h"
#include "PolyVector4.h"
#include "MathUtil.h"

using namespace Polycode;

// Specifies that this class is a Hammer co-ordinate space equivalent of the template class.
template<typename T>
class IHammerEquivalent
{
public:
	virtual ~IHammerEquivalent() {}
	
	// The input is a Polycode class and this data should be converted to a Hammer format within this class.
	// Eg. Converting a Polycode world vector (input) to a Hammer world vector (this).
	virtual void toHammerEquivalent(const T &other) = 0;
	
	// This class' data should be output as a Polycode class.
	// Eg. Converting a Hammer world vector (this) to a Polycode world vector (output).
	virtual T fromHammerEquivalent() const = 0;
};

// TODO: Further operator overloading
class CVector3 : public Vector3, public IHammerEquivalent<Vector3>
{
public:
	// Input is already in Hammer format.
	CVector3(Number x, Number y, Number z) : Vector3(x, y, z)
	{
	}
	
	// Input is already in Hammer format.
	CVector3() : Vector3()
	{
	}
	
	virtual void toHammerEquivalent(const Vector3 &other)
	{
		// Hammer X,Y,Z = Polycode X,-Z,Y
		y = -other.z;
		z = other.y;
	}
	
	virtual Vector3 fromHammerEquivalent() const
	{
		// Polycode X,Y,Z = Hammer X,Z,-Y
		return Vector3(x, z, -y);
	}
};

// TODO: Further operator overloading
class CVector4 : public Vector4, public IHammerEquivalent<Vector4>
{
public:
	// Input is already in Hammer format.
	CVector4(Number x, Number y, Number z, Number w) : Vector4(x, y, z, w)
	{
	}
	
	// Input is already in Hammer format.
	CVector4() : Vector4()
	{
	}
	
	virtual void toHammerEquivalent(const Vector4 &other)
	{
		y = -other.z;
		z = other.y;
	}
	
	virtual Vector4 fromHammerEquivalent() const
	{
		return Vector4(x, z, -y, w);
	}
};

/*
 * (0, 0, 0) points down the positive X axis, with the up vector pointing up the positive vertical Z axis.
 * (-90, 0, 0) points straight up the positive Z axis, with the up vector pointing down the negative X axis.
 * (90, 0, 0) points straight down the negative Z axis, with the up vector pointing up the positive X axis.
 * (0, 90, 0) points up the positive Y axis, with the up vector still pointing up the positive vertical Z axis.
 * (0, 0, 90) points up the positive X axis with the up vector pointing down the negative Y axis.
 * (0, 0, -90) points up the positive X axis with the up vector pointing up the positive Y axis.
 *
 * Therefore the pitch, yaw and roll can be thought of in the following way:
 *
 * Yaw should always be applied around the world Z axis. A yaw of 0 points down the world X axis; increasing the yaw
 * rotates anticlockwise around the world Z axis.
 *
 * Pitch is applied relative to the camera after yaw. Negative pitch values rotate upwards to point up the Z axis; positive values rotate downwards.
 *
 * Roll is applied relative to the camera after pitch. Positive roll values rotate clockwise around the vector specified by pitch and yaw;
 * negative values rotate anticlockwise.
 * Alternatively, you can think of applying the roll, then pitch, then yaw, around world axes at the point of application.
 
 For Polycode, roll is applied along the Z axis. When looking up the Z axis, positive roll rotates anticlockwise and negative roll
 rotates clockwise.
 Yaw is applies along the Y axis. When looking up the Y axis, positive yaw rotates anticlockwise and negative roll rotates
 clockwise. 0 yaw points down the negative Z axis.
 Pitch is applies along the X axis. When looking up the X axis, positive pitch rotates clockwise and negative roll rotates
 anticlockwise.
 */

// TODO: Further operator overloading
class CEulerAngle : public Vector3, public IHammerEquivalent<Vector3>
{
public:
	// Input is already in Hammer format.
	CEulerAngle(Number pitch, Number yaw, Number roll) : Vector3(pitch, yaw, roll)
	{
	}
	
	// Input is already in Hammer format.
	CEulerAngle() : Vector3()
	{
	}
	
	virtual void toHammerEquivalent(const Vector3 &other)
	{
		// TODO: Rules for converting angles
	}
	
	virtual Vector3 fromHammerEquivalent() const
	{
		// TODO: Rules for converting angles
		return Vector3();
	}
};

#endif	// IHAMMEREQUIVALENT_H