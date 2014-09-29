#include "BasicPhysicsObject.h"
#include <cmath>
using namespace Frost;

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
	_forcesCurrent = true;
	_netForce += f_w;
}

void BasicPhysicsObject::addForceAtPoint(const Vect3& f_w, const Vect3& pt_world)
{
	_forcesCurrent = true;
	// Break up force into parallel and perpendicular components.
	try
	{
		Vect3Normal armNormal = pt_world - GetPos();
		Vect3 originWorldForce = armNormal * (f_w * armNormal);
		Vect3 originWorldTorque = CrossProduct(f_w, armNormal);

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
	_forcesCurrent;
	_netTorque += t_w;
}

// Updates the object by clearing all accumulated forces,
//  and applying them to the linear and angular velocity of the object.
void BasicPhysicsObject::update(float dt)
{
	_forcesCurrent = false;
	_lastForce = _netForce;
	_lastTorque = _netTorque;

	Vect3 frameAcc = _netForce * _invMass;
	Vect3 frameTorque = _invInertiaTensor * _netTorque;

	_linearVelocity += frameAcc * dt;
	_angularVelocity += frameTorque * dt;

	// Impose drag...
	_linearVelocity *= std::pow((1.f - _linearDrag), dt);
	_angularVelocity *= std::pow((1.f - _angularDrag), dt);

	_netForce = MathConstants::VECTOR_ZERO;
	_netTorque = MathConstants::VECTOR_ZERO;

	SetPos(GetPos() + (_linearVelocity * dt));
	SetOrientation(GetOrientation() + (_angularVelocity * dt));
}

void BasicPhysicsObject::impulse(const Vect3& applicationPoint, const Vect3& distance)
{
	Vect3Normal armNormal = applicationPoint - GetPos();
	Vect3 worldLinearMotion = armNormal * (distance * armNormal);
	Vect3 worldAngularMotion = CrossProduct(distance, armNormal);

	Move(worldLinearMotion);
	SetOrientation(GetOrientation() + worldAngularMotion);
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