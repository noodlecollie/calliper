#ifndef CCAMERAMOTION_H
#define CCAMERAMOTION_H

#include "Polycode.h"

using namespace Polycode;

class CCameraMotion
{
public:
	CCameraMotion(Camera* camera);
	CCameraMotion();

	Camera* camera;

	void advance(Number secs);

	void pressForward(bool pressed);
	void pressBackward(bool pressed);
	void pressLeft(bool pressed);
	void pressRight(bool pressed);
	
	void stopMovement();

private:
	enum MotionKey
	{
		Forward = 0x1,
		Backward = 0x2,
		Left = 0x4,
		Right = 0x8
	};

private:
	bool hasKeyPressed() const;
	void modifyKey(MotionKey key, bool pressed);
	void calculateCurrentVelocity(Number secs);
	Vector3 directionFromKeys() const;
	Number calculateMotion(Number dir, Number u, Number acc, Number dec, Number t);

	Number	m_flMaxSpeed;
	Number	m_flTimeToMaxSpeed;
	Number	m_flTimeToStop;
	Vector3	m_vecVelocity;		// In units per second
	int		m_iPressedKeys;
	int		m_iLastPressedKeys;
};

#endif	// CCAMERAMOTION_H