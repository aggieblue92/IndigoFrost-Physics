#include "PlayerMotionForce.h"
#include <cmath>
using namespace Frost;

PlayerMotionForce::PlayerMotionForce(const Vect3& maxVelocity, float accTime)
: _direction(maxVelocity == MathConstants::VECTOR_ZERO ? MathConstants::VECTOR_UNIT_X : maxVelocity)
, _speed(maxVelocity.Magnitude())
, _timeToMaxSpeed(accTime)
{}

PlayerMotionForce::PlayerMotionForce(const PlayerMotionForce& o)
: _direction(o._direction)
, _speed(o._speed)
, _timeToMaxSpeed(o._timeToMaxSpeed)
{}

void PlayerMotionForce::setDirectionAndSpeed(const Vect3& dirn, float speed)
{
	try
	{
		_direction = dirn;
		_speed = speed;
	}
	catch (ZeroMagnitudeException&)
	{
		_speed = 0.f;
	}
}

void PlayerMotionForce::setMaxVelocity(float maxVelocity)
{
	_speed = maxVelocity;
}

void PlayerMotionForce::setDirection(const Vect3& dirn)
{
	try
	{
		_direction = dirn;
	}
	catch (ZeroMagnitudeException&)
	{
		_speed = 0.f;
	}
}

void PlayerMotionForce::setTimeToMaxVelocity(float time)
{
	_timeToMaxSpeed = time;
}

Vect3Normal PlayerMotionForce::getDirection() const
{
	return _direction;
}

Vect3 PlayerMotionForce::getDirectionAndSpeed() const
{
	return _direction * _speed;
}

float PlayerMotionForce::getVelocity() const
{
	return _speed;
}

float PlayerMotionForce::getTimeToMaxVelodity() const
{
	return _timeToMaxSpeed;
}

void PlayerMotionForce::ApplyForce(IPhysicsObject* me, float dt)
{
	// We'll be taking the inverse of the suration, so make sure
	//  that in fact it's greater than zero.
	if (dt == 0.f)
		return;

	// Also don't bother if the object is immutable
	if (me->getInverseMass() == 0.f)
		return;

	// If the object is already traveling at or above max speed,
	//  don't do anything else here.
	if (me->getLinearVelocity() * _direction >= _speed)
		return;

	// Acceleration (for a linear curve):
	// a(t) = MAX_VEL / dt
	Vect3 acc = _direction * _speed / _timeToMaxSpeed;

	// Divide by the inverse mass so that the acceleration is constant
	//  no matter what the mass of the object... We gave
	//  time to acceleration, not a force value, no?
	me->addForceAtOrigin(acc / me->getInverseMass());
}

IForce* PlayerMotionForce::getNewForcePtr() const
{
	return new PlayerMotionForce(*this);
}