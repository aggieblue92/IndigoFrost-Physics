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
#include "BasicPhysicsObject.h"
#include <cmath>

namespace Frost {

BasicPhysicsObject::BasicPhysicsObject(float invMass, const Matrix& invInertiaTensor,
	float linearDrag, float angularDrag,
	const FLOAT3& initPos,
	const Quaternion& initOrientation,
	const FLOAT3& initLinearVelocity,
	const FLOAT3& initAngularVelocity)
	: IPhysicsObject(initPos, initOrientation,
	initLinearVelocity, initAngularVelocity,
	invMass, invInertiaTensor)
	, _linearDrag(linearDrag)
	, _angularDrag(angularDrag)
{
	if (_linearDrag < 0.f || _angularDrag < 0.f)
	{
		DebugLogger::err("Error in creation of BasicPhysicsObject - linear or angular drag parameter out of range. Throwing exception.\n");
		throw OutOfBoundsException();
	}
}

BasicPhysicsObject::BasicPhysicsObject(const BasicPhysicsObject& o)
: IPhysicsObject(o)
, _linearDrag(o._linearDrag)
, _angularDrag(o._angularDrag)
{}

void BasicPhysicsObject::addForceAtOrigin(const Vect3& f_w)
{
	_netForce += f_w;
}

void BasicPhysicsObject::addForceAtPoint(const Vect3& f_w, const Vect3& pt_world)
{
	// Break up force into parallel and perpendicular components.
	try
	{
		Vect3Normal armNormal = pt_world - getPos();
		Vect3 originWorldForce = armNormal * (armNormal * f_w);
		Vect3 originWorldTorque = armNormal % f_w;

		if(DebugLogger::isFlagSet(DebugLogger::DEBUG_LEVEL_DEBUG_TO_FILE | DebugLogger::DEBUG_LEVEL_DEBUG_TO_COUT))
		{
			std::stringstream ss("");
			ss << "BasicPhysicsObject: Origin force: " << originWorldForce << ", torque: " << originWorldTorque << std::endl;
			DebugLogger::debug(ss.str());
		}

		addForceAtOrigin(originWorldForce);
		addTorqueAtOrigin(originWorldTorque);
	}
	catch (ZeroMagnitudeException)
	{
		// The force is actually being applied at the origin... So just
		//  apply it at the origin.
		addForceAtOrigin(f_w);
	}
}

void BasicPhysicsObject::addTorqueAtOrigin(const Vect3& t_w)
{
	_netTorque += t_w;
}

// Updates the object by clearing all accumulated forces,
//  and applying them to the linear and angular velocity of the object.
void BasicPhysicsObject::update(float dt)
{
	Vect3 frameAcc = _netForce * _invMass;
	Vect3 frameTorque = _invInertiaTensor * _netTorque;

	if(DebugLogger::isFlagSet(DebugLogger::DEBUG_LEVEL_LOG_TO_FILE | DebugLogger::DEBUG_LEVEL_LOG_TO_CLOG))
	{
		std::stringstream ss("");
		ss << "Net frame force: " << _netForce << ", torque: " << _netTorque << std::endl;
		DebugLogger::log(ss.str());
	}

	_linearVelocity += frameAcc * dt;
	_angularVelocity += frameTorque * dt;

	// Impose drag...
	_linearVelocity *= std::pow((1.f - _linearDrag), dt);
	_angularVelocity *= std::pow((1.f - _angularDrag), dt);

	_netForce = MathConstants::VECTOR_ZERO;
	_netTorque = MathConstants::VECTOR_ZERO;

	setPos(getPos() + (_linearVelocity * dt));
	setOrientation(getOrientation() + (_angularVelocity * dt));
}

void BasicPhysicsObject::impulse(const Vect3& applicationPoint, const Vect3& distance)
{
	Vect3Normal armNormal = applicationPoint - getPos();
	Vect3 worldLinearMotion = armNormal * (armNormal * distance);
	Vect3 worldAngularMotion = armNormal % distance;

	move(worldLinearMotion);
	setOrientation(getOrientation() + worldAngularMotion);
}

float BasicPhysicsObject::getLinearDrag() const
{
	return _linearDrag;
}

float BasicPhysicsObject::getAngularDrag() const
{
	return _angularDrag;
}

void BasicPhysicsObject::setLinearDrag(float ld)
{
	if (ld < 0.f)
		throw OutOfBoundsException();
	_linearDrag = ld;
}

void BasicPhysicsObject::setAngularDrag(float ad)
{
	if (ad < 0.f)
		throw OutOfBoundsException();
	_angularDrag = ad;
}

}