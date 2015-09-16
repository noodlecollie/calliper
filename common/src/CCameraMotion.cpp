#include "CCameraMotion.h"
#include "Polycode.h"
#include "CalliperGlobals.h"

#define ACCELERATION (m_flMaxSpeed / m_flTimeToMaxSpeed)
#define DECELERATION (-m_flMaxSpeed / m_flTimeToStop)

CCameraMotion::CCameraMotion(Camera* camera)
{
	this->camera = camera;
	m_flMaxSpeed = 20.0f;
	m_flTimeToMaxSpeed = 0.2f;
	m_flTimeToStop = 0.2f;
	m_iPressedKeys = 0;
	m_iLastPressedKeys = 0;
}

CCameraMotion::CCameraMotion()
{
	camera = NULL;
}

bool CCameraMotion::hasKeyPressed() const
{
	return m_iPressedKeys != 0;
}

void CCameraMotion::modifyKey(MotionKey key, bool pressed)
{
	if (pressed)
		m_iPressedKeys |= key;
	else
		m_iPressedKeys &= ~key;
}

void CCameraMotion::pressForward(bool pressed)
{
	modifyKey(Forward, pressed);
}

void CCameraMotion::pressBackward(bool pressed)
{
	modifyKey(Backward, pressed);
}

void CCameraMotion::pressLeft(bool pressed)
{
	modifyKey(Left, pressed);
}

void CCameraMotion::pressRight(bool pressed)
{
	modifyKey(Right, pressed);
}

void CCameraMotion::pressUp(bool pressed)
{
	modifyKey(Up, pressed);
}

void CCameraMotion::pressDown(bool pressed)
{
	modifyKey(Down, pressed);
}

Number CCameraMotion::calculateMotion(Number dir, Number u, Number acc, Number dec, Number t)
{
	if (dir == 0)
	{
		// No buttons pressed, so decelerate.
		if (u < 0) dec = -dec;
		Number v = u + (dec * t);
		
		// If the deceleration has resulted in motion in the opposite direction, clip to 0.
		if ((u >= 0 && v < 0) || (u <= 0 && v > 0)) v = 0;
		return v;
	}
	else
	{
		// If the intended direction of motion is opposite to the current direction on this axis,
		// increase the acceleration.
		if ((dir > 0 && u < 0) || (dir < 0 && u > 0))
		{
			acc *= 2;
		}
		
		// Accelerate and clip to our max axis speed.
		if (dir < 0) acc = -acc;
		Number v = u + (acc * t);
		if (v > m_flMaxSpeed) v = m_flMaxSpeed;
		return v;
	}
}

// v = u + at
void CCameraMotion::calculateCurrentVelocity(Number secs)
{
	Vector3 dir = directionFromKeys();
	
	// Handle axes independently.
	m_vecVelocity.x = calculateMotion(dir.x, m_vecVelocity.x, ACCELERATION, DECELERATION, secs);
	m_vecVelocity.y = calculateMotion(dir.y, m_vecVelocity.y, ACCELERATION, DECELERATION, secs);
	m_vecVelocity.z = calculateMotion(dir.z, m_vecVelocity.z, ACCELERATION, DECELERATION, secs);
	
	// TODO: Clip vector length to max speed? We'll exceed it otherwise by a factor of root 2 (I think)
	// when we're moving diagonally. May not be worth it.
	Vector3 p = camera->getPosition();
	Vector3 a = camera->getRotationEuler();
	Logger::log("Camera position: " PR_VECTOR3 "\nCamera angles: " PR_VECTOR3 "\n", FM_VECTOR3(p), FM_VECTOR3(a));
}

Vector3 CCameraMotion::directionFromKeys() const
{
	int x = 0;
	int y = 0;
	int z = 0;

	if ((m_iPressedKeys & Forward) == Forward)
		y++;
	if ((m_iPressedKeys & Backward) == Backward)
		y--;

	if ((m_iPressedKeys & Right) == Right)
		x++;
	if ((m_iPressedKeys & Left) == Left)
		x--;
	
	if ((m_iPressedKeys & Up) == Up)
		z++;
	if ((m_iPressedKeys & Down) == Down)
		z--;

	Vector3 v(x, y, z);
	return v;
}

void CCameraMotion::advance(Number secs)
{
	// Update our velocity vector.
	calculateCurrentVelocity(secs);
	
	// If there is no camera, return.
	if ( !camera ) return;
	
	// Calculate the translation vector for this time interval.
	//Number seconds = (Number)msec / 1000.0f;
	Vector3 translation = m_vecVelocity * secs;
	//Logger::log("Translation: %f %f %f\n", translation.x, translation.y, translation.z);
	
	// Get the local->world matrix.
	Matrix4 mat = camera->getConcatenatedMatrix();
	
	// Transform our vector to a world space vector.
	translation = mat.rotateVector(translation);
	
	// Translate the camera.
	camera->Translate(translation);
	
	// Cache the last key state.
	m_iLastPressedKeys = m_iPressedKeys;
}

void CCameraMotion::stopMovement()
{
	m_vecVelocity = Vector3(0,0,0);
	m_iPressedKeys = 0;
}