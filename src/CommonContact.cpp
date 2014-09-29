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
	if (attachedObj == 0 || otherObject == 0)
	{
		throw NullObjectException();
	}
}

bool CommonContact::Resolve(float dt)
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
	Vect3 affAngular = Frost::CrossProduct(_affectedObject->getAngularVelocity(), _affectedObject->GetPos() - _objCollisionPoint);
	Vect3 relativeVelocityOfCollisionPoint = _affectedObject->getLinearVelocity()
		+ Frost::CrossProduct(_affectedObject->getAngularVelocity(), _affectedObject->GetPos() - _objCollisionPoint)
		- _otherObject->getLinearVelocity()
		- Frost::CrossProduct(_otherObject->getAngularVelocity(), _objCollisionPoint - _otherObject->GetPos());

	// If it is more, apply bounciness to it and reverse it.
	float velocityIntoNormal = relativeVelocityOfCollisionPoint * _contactNormal;
	float roughFrameAcc = _affectedObject->getInverseMass() * _contactNormal * _affectedObject->getNetForce() * dt * dt;
	//float twoFrames = normalForce.Magnitude() * _affectedObject->getInverseMass() * 1.8f;
	if (velocityIntoNormal > roughFrameAcc)
	{
		// TODO: This is dangerous, because it assumes a constant framerate, which is
		//  not necessarily guaranteed. Perhaps you should implement an addForceWithTime or something?
		Vect3 velocityVecIntoNormal = _contactNormal * velocityIntoNormal;
		_affectedObject->addForceAtPoint(-_affectedObject->getMass() * (velocityVecIntoNormal * (1.f + _bounciness)) / dt, _objCollisionPoint);
		_affectedObject->update(dt);
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
	if (x.SquareMagnitude() != 0.f)
	{
		Vect3Normal pointDirection = CrossProduct(_contactNormal, Vect3Normal(x));

		// Adjust the rolling vs. moving speed
		Vect3 vt = pointDirection * relativeVelocityOfCollisionPoint.Magnitude();
		Vect3 va = vt * std::pow(_friction, dt);
		va /= -2.f;

		//// Adjust the linear velocity portion...
		_affectedObject->addForceAtOrigin(va * -1.f);
		_affectedObject->addForceAtPoint(va, _objCollisionPoint);
		//_affectedObject->setLinearVelocity(_affectedObject->getLinearVelocity() - va);
	}

	return true;
}