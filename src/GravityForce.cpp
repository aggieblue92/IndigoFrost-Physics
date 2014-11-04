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
#include "GravityForce.h"
using namespace Frost;

GravityForce::GravityForce(float objMass)
: _gravityDirection(MathConstants::VECTOR_UNIT_Y * -1.f)
, _gravityAcceleration(9.81f)
{}

GravityForce::GravityForce(const Vect3Normal& dirn, float mag)
: _gravityDirection(dirn)
, _gravityAcceleration(mag)
{}

GravityForce::GravityForce(const GravityForce& o)
: _gravityDirection(o._gravityDirection)
, _gravityAcceleration(o._gravityAcceleration)
{}

void GravityForce::setMagnitude(float nm)
{
	_gravityAcceleration = nm;
}

void GravityForce::setGravityDirection(const Vect3Normal& dn)
{
	_gravityDirection = dn;
}

Vect3Normal GravityForce::getGravityDirection()
{
	return _gravityDirection;
}

float GravityForce::getGravityMagnitude()
{
	return _gravityAcceleration;
}

void GravityForce::applyForce(std::shared_ptr<IPhysicsObject> affectedObject, float duration)
{
	if (affectedObject->getInverseMass() == 0.f)
		return;
	else
		affectedObject->addForceAtOrigin(_gravityDirection * _gravityAcceleration * (1.f / affectedObject->getInverseMass()));
}