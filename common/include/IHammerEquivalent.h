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
	
	CVector3(const Vector3 &other) : Vector3()
	{
		toHammerEquivalent(other);
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
	
	CVector4(const Vector4 &other) : Vector4()
	{
		toHammerEquivalent(other);
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
	Polycode:
	Xp = Xh		Pitch:	+ -> clockwise around Xp = clockwise around Xh
	Yp = Zh		Yaw:	+ -> Clockwise around Yp = clockwise around Zh
	Zp = -Yh	Roll:	+ -> Clockwise around Zp = anticlockwise around Yh
 
	Hammer:
	Xh = Xp		Pitch:	+ -> Clockwise around Yh = anticlockwise around Zp
	Yh = -Zp	Yaw:	+ -> Clockwise around Zh = clockwise around Yp
	Zh = Yp		Roll:	+ -> Clockwise around Xh = clockwise around Xp
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