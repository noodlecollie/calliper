#ifndef CCAMERACONTROLLER_H
#define CCAMERACONTROLLER_H

#include "Polycode.h"

using namespace Polycode;

class CCameraController
{
public:
	CCameraController(Camera* camera);
	CCameraController();

	Camera* camera;

	void advance(int msec);

	void pressForward(bool pressed);
	void pressBackward(bool pressed);
	void pressLeft(bool pressed);
	void pressRight(bool pressed);

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
	Number calculateAcceleration() const;
	void calculateCurrentVelocity(int msec);
	Vector3 directionFromKeys() const;

	Number	m_flMaxSpeed;
	Number	m_flTimeToMaxSpeed;
	Vector3	m_vecVelocity;
	int		m_iPressedKeys;
}

#endif	// CCAMERACONTROLLER_H