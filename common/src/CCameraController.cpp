#include "CCameraController.h"
#include "Polycode.h"

CCameraController::CCameraController(Camera* camera)
{
	this->camera = camera;
	m_flMaxSpeed = 100.0f;
	m_flTimeToMaxSpeed = 0.4f;
}

CCameraController::CCameraController()
{
	camera = NULL;
}

bool CCameraController::hasKeyPressed() const
{
	return m_iPressedKeys != 0;
}

void CCameraController::modifyKey(MotionKey key, bool pressed)
{
	if (pressed)
		m_iPressedKeys |= Forward;
	else
		m_iPressedKeys &= ~Forward;
}

void CCameraController::pressForward(bool pressed)
{
	modifyKey(Forward, pressed);
}

void CCameraController::pressBackward(bool pressed)
{
	modifyKey(Backward, pressed);
}

void CCameraController::pressLeft(bool pressed)
{
	modifyKey(Left, pressed);
}

void CCameraController::pressRight(bool pressed)
{
	modifyKey(Right, pressed);
}

void CCameraController::calculateCurrentVelocity(int msec)
{
	Number currentSpeed = m_vecVelocity.length();

	// If no keys are pressed, decelerate.
	if (!hasKeyPressed())
	{
		// v = u + at
		// u = currentSpeed
		// a = deceleration
		// t = time delta
		// v = ?
		Number a = -calculateAcceleration();
		Number t = (Number)msec / 1000.0f;
		Number v = currentSpeed + (a * t);
		if (v < 0.0f) v = 0.0f;

		m_vecVelocity = m_vecVelocity * v;
		return;
	}

	if (currentSpeed > m_flMaxSpeed) currentSpeed = m_flMaxSpeed;
	if (currentSpeed == m_flMaxSpeed) return;

	// Get a vector from the currently pressed keys.
	// The direction is relative to the camera: X is right and Y is forward.
	Vector3 dir = directionFromKeys();

	// TODO
}

Number CCameraController::calculateAcceleration() const
{
	// We accelerate to maxSpeed m/s over time seconds.
	// This means the acceleration = maxSpeed/time.
	return m_flMaxSpeed / m_flTimeToMaxSpeed;
}

Vector3 CCameraController::directionFromKeys() const
{
	int x = 0;
	int y = 0;

	if ((m_iPressedKeys & Forward) == Forward)
		y++;
	if ((m_iPressedKeys & Backward) == Backward)
		y--;

	if ((m_iPressedKeys & Right) == Right)
		x++;
	if ((m_iPressedKeys & Left) == Left)
		x--;

	Vector3 v(x, y, 0);
	v.Normalize();
	return v;
}