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

#include "CommonContact.h"
#include <cmath>
using namespace Frost;

CommonContact::CommonContact(const Vect3& pt, const Vect3& penetration, float bounciness, float friction, IPhysicsObject* attachedObj, IPhysicsObject* otherObject)
: IContact(pt, penetration)
, _affectedObject(attachedObj)
, _friction(friction)
, _bounciness(bounciness)
, _otherObject(otherObject)
{
	// TODO: Reconsider - the other object might be able to be null...
	if (attachedObj == 0 || otherObject == 0)
	{
		throw NullObjectException();
	}
}

bool CommonContact::resolve(float dt)
{
	// Get out if the affected object is immutable
	if (!_affectedObject->isMutable())
		return true;

	// Identify the normal force of the object against the
	//  surface of contact
	Vect3 normalForce = (_affectedObject->getNetForce() * _contactNormal) * _contactNormal * -1.f;

	// Via an impulse, resolve interpenetration
	_affectedObject->impulse(_objCollisionPoint, _contactNormal * _contactMagnitude * -1.f);

	// Look at velocity going into the contact normal - if it is the kind of velocity that
	//  can be caused by one or two frames, just null it out.
	Vect3 affLinear = _affectedObject->getLinearVelocity();
	Vect3 affAngular = Frost::CrossProduct(_affectedObject->getAngularVelocity(), _affectedObject->getPos() - _objCollisionPoint);
	Vect3 relativeVelocityOfCollisionPoint = _affectedObject->getLinearVelocity()
		+ Frost::CrossProduct(_affectedObject->getAngularVelocity(), _affectedObject->getPos() - _objCollisionPoint)
		- _otherObject->getLinearVelocity()
		- Frost::CrossProduct(_otherObject->getAngularVelocity(), _objCollisionPoint - _otherObject->getPos());

	// If it is more, apply bounciness to it and reverse it.
	float velocityIntoNormal = relativeVelocityOfCollisionPoint * _contactNormal;
	float roughFrameAcc = _affectedObject->getInverseMass() * _contactNormal * _affectedObject->getNetForce() * dt * dt;
	//float twoFrames = normalForce.Magnitude() * _affectedObject->getInverseMass() * 1.8f;
	if (velocityIntoNormal > roughFrameAcc)
	{
		Vect3 velocityVecIntoNormal = _contactNormal * velocityIntoNormal;
		_affectedObject->addForceAtPoint(-_affectedObject->getMass() * (velocityVecIntoNormal * (1.f + _bounciness)) / dt, _objCollisionPoint);
	}

	// If it isn't, just apply the normal force
	else
	{
		_affectedObject->addForceAtPoint(normalForce, _objCollisionPoint);
	}

	// Find motion along surface, apply friction force to it.
	//  We do this by reconciling motion due to angular velocity, and motion due to linear velocity,
	//  in any direction perpendicular to the collision normal.
	Vect3 x = CrossProduct(Vect3Normal(relativeVelocityOfCollisionPoint), _contactNormal);
	if (x.squareMagnitude() != 0.f)
	{
		Vect3Normal pointDirection = CrossProduct(_contactNormal, Vect3Normal(x));
		Vect3 frictionForce;

		frictionForce = pointDirection * (_friction * normalForce).magnitude() * -1.f;

		_affectedObject->addForceAtPoint(frictionForce, _objCollisionPoint);
	}

	return true;
}