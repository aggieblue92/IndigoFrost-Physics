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
{}

IPhysicsObject::IPhysicsObject(const IPhysicsObject& toCopy)
: Movable(toCopy)
, _linearVelocity(toCopy._linearVelocity)
, _angularVelocity(toCopy._angularVelocity)
, _invMass(toCopy._invMass)
, _invInertiaTensor(toCopy._invInertiaTensor)
, _netForce(toCopy._netForce)
, _netTorque(toCopy._netTorque)
{}

IPhysicsObject::IPhysicsObject(const Movable& inMovable)
: Movable(inMovable)
, _linearVelocity(0.f, 0.f, 0.f)
, _angularVelocity(0.f, 0.f, 0.f)
, _invMass(0.f)
, _invInertiaTensor(MathConstants::MATRIX_ZERO)
, _netForce(0.f, 0.f, 0.f)
, _netTorque(0.f, 0.f, 0.f)
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
	return _netForce;
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