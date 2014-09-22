#include "IPhysicsObject.h"
using namespace Frost;

IPhysicsObject::IPhysicsObject()
: Movable()
, _linearVelocity(0.f, 0.f, 0.f)
, _angularVelocity(0.f, 0.f, 0.f)
, _invMass(0.f)
, _invInertiaTensor(MathConstants::MATRIX_ZERO)
{}

IPhysicsObject::IPhysicsObject(const IPhysicsObject& toCopy)
: Movable(toCopy)
, _linearVelocity(toCopy._linearVelocity)
, _angularVelocity(toCopy._angularVelocity)
, _invMass(toCopy._invMass)
, _invInertiaTensor(toCopy._invInertiaTensor)
{}

IPhysicsObject::IPhysicsObject(const Movable& inMovable)
: Movable(inMovable)
, _linearVelocity(0.f, 0.f, 0.f)
, _angularVelocity(0.f, 0.f, 0.f)
, _invMass(0.f)
, _invInertiaTensor(MathConstants::MATRIX_ZERO)
{}

IPhysicsObject::IPhysicsObject(const FLOAT3& pos, const Quaternion& quat,
	const Vect3& linVel,
	const Vect3& angVel,
	float invMass,
	const Matrix& invIT)
: Movable(pos, quat)
, _linearVelocity(linVel)
, _angularVelocity(angVel)
, _invMass(invMass)
, _invInertiaTensor(invIT)
{}

Vect3 IPhysicsObject::getLinearVelocity() const
{
	return _linearVelocity;
}

void IPhysicsObject::setLinearVelocity(const FLOAT3& newLinearVelocity)
{
	_linearVelocity = newLinearVelocity;
}

Vect3 IPhysicsObject::getAngularVelocity() const
{
	return _angularVelocity;
}

void IPhysicsObject::setAngularVelocity(const FLOAT3& newAngularVelocity)
{
	_angularVelocity = newAngularVelocity;
}

Vect3 IPhysicsObject::getNetForce() const
{
	return _netForce;
}

Vect3 IPhysicsObject::getNetTorque() const
{
	return _netTorque;
}

float IPhysicsObject::getInverseMass() const
{
	return _invMass;
}

void IPhysicsObject::setInverseMass(float newInverseMass)
{
	_invMass = newInverseMass;
}

Matrix IPhysicsObject::getInverseInertiaTensor() const
{
	return _invInertiaTensor;
}

void IPhysicsObject::setInverseInertiaTensor(const Matrix& n)
{
	_invInertiaTensor = n;
}