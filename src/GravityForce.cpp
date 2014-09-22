#include "GravityForce.h"
using namespace Frost;

GravityForce::GravityForce(float objMass)
: _gravityDirection(MathConstants::VECTOR_UNIT_Y * -1.f)
, _gravityAcceleration(9.81f)
{}

GravityForce::GravityForce(const Vect3Normal& dirn, float mag)
: _gravityAcceleration(mag)
, _gravityDirection(dirn)
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

void GravityForce::ApplyForce(IPhysicsObject* affectedObject, float duration)
{
	if (affectedObject->getInverseMass() == 0.f)
		return;
	else
		affectedObject->addForceAtOrigin(_gravityDirection * _gravityAcceleration * (1.f / affectedObject->getInverseMass()));
}
