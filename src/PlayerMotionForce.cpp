/*
This source file is part of the Indigo Frost physics engine

The MIT License (MIT)

Copyright (c) 2014 Kamaron Peterson (aggieblue92)

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "PlayerMotionForce.h"
#include <cmath>
using namespace Frost;

PlayerMotionForce::PlayerMotionForce(const Vect3& maxVelocity, float accTime)
: _direction(maxVelocity == MathConstants::VECTOR_ZERO ? MathConstants::VECTOR_UNIT_X : maxVelocity)
, _speed(maxVelocity.magnitude())
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

void PlayerMotionForce::applyForce(IPhysicsObject* me, float dt)
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