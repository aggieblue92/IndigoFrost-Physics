#include "IPhysicsObject.h"
using namespace Frost;

IPhysicsObject::IPhysicsObject()
: Movable()
, _linearVelocity(0.f, 0.f, 0.f)
, _angularVelocity(0.f, 0.f, 0.f)
, _invMass(0.f)
, _invInertiaTensor(MathConstants::MATRIX_ZERO)
, _netForce(0.f, 0.f, 0.f)
, _netTorque(0.f, 0.f, 0.f)
, _lastForce(0.f, 0.f, 0.f)
, _lastTorque(0.f, 0.f, 0.f)
, _forcesCurrent(true)
{}

IPhysicsObject::IPhysicsObject(const IPhysicsObject& toCopy)
: Movable(toCopy)
, _linearVelocity(toCopy._linearVelocity)
, _angularVelocity(toCopy._angularVelocity)
, _invMass(toCopy._invMass)
, _invInertiaTensor(toCopy._invInertiaTensor)
, _netForce(toCopy._netForce)
, _netTorque(toCopy._netTorque)
, _lastForce(toCopy._lastForce)
, _lastTorque(toCopy._lastTorque)
, _forcesCurrent(toCopy._forcesCurrent)
{}

IPhysicsObject::IPhysicsObject(const Movable& inMovable)
: Movable(inMovable)
, _linearVelocity(0.f, 0.f, 0.f)
, _angularVelocity(0.f, 0.f, 0.f)
, _invMass(0.f)
, _invInertiaTensor(MathConstants::MATRIX_ZERO)
, _netForce(0.f, 0.f, 0.f)
, _netTorque(0.f, 0.f, 0.f)
, _lastForce(0.f, 0.f, 0.f)
, _lastTorque(0.f, 0.f, 0.f)
, _forcesCurrent(true)
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
, _netForce(0.f, 0.f, 0.f)
, _netTorque(0.f, 0.f, 0.f)
, _lastForce(0.f, 0.f, 0.f)
, _lastTorque(0.f, 0.f, 0.f)
, _forcesCurrent(true)
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
	if (_forcesCurrent)
		return _netForce;
	else
		return _lastForce;
}

Vect3 IPhysicsObject::getNetTorque() const
{
	if (_forcesCurrent)
		return _netTorque;
	else
		return _lastTorque;
}

float IPhysicsObject::getInverseMass() const
{
	return _invMass;
}

float IPhysicsObject::getMass() const
{
	if (_invMass == 0.f)
	{
		throw DivByZeroException();
	}
	else
	{
		return 1.f / _invMass;
	}
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

bool IPhysicsObject::isMutable() const
{
	return _invMass != 0.f;
}

void IPhysicsObject::clearForces()
{
	_lastForce = _netForce;
	_lastTorque = _netTorque;
	_forcesCurrent = false;
	_netForce = MathConstants::VECTOR_ZERO;
	_netTorque = MathConstants::VECTOR_ZERO;
}