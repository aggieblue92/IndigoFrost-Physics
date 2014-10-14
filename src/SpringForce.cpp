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
#include "SpringForce.h"
using namespace Frost;

SpringForce::SpringForce(const Vect3& lcp,
	IPhysicsObject* o,
	const Vect3& ocp,
	float k,
	float x)
	: _otherLocalConnectionPoint(ocp)
	, _restLength(x)
	, _springConstant(k)
	, _otherObjectInvolved(o)
	, _localConnectionPoint(lcp)
{
	if (o == 0)
	{
		throw NullObjectException();
	}
}

SpringForce::SpringForce(const SpringForce& o)
: _otherLocalConnectionPoint(o._otherLocalConnectionPoint)
, _restLength(o._restLength)
, _springConstant(o._springConstant)
, _otherObjectInvolved(o._otherObjectInvolved)
, _localConnectionPoint(o._localConnectionPoint)
{}

void SpringForce::applyForce(IPhysicsObject* me, float dt)
{
	// Calculate the two ends in world space
	Vect3 lws = me->getTransformMatrix() * _localConnectionPoint;
	Vect3 rhs = _otherObjectInvolved->getTransformMatrix() * _otherLocalConnectionPoint;

	// Calculate the vector of the spring...
	Vect3 distance = lws - rhs;

	// Calculate the magnitude of the force...
	float magnitude = (distance.magnitude() - _restLength) * _springConstant;

	// Calculate the final force and apply it
	me->addForceAtPoint(Vect3Normal(distance) * magnitude, lws);
	_otherObjectInvolved->addForceAtPoint(Vect3Normal(distance) * -magnitude, rhs);
}

IForce* SpringForce::getNewForcePtr() const
{
	return new SpringForce(*this);
}