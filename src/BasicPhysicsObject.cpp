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
	_netForce += f_w;
}

void BasicPhysicsObject::addForceAtPoint(const Vect3& f_w, const Vect3& pt_world)
{
	// Break up force into parallel and perpendicular components.
	Vect3Normal armNormal = pt_world - GetPos();
	Vect3 originWorldForce = armNormal * (f_w * armNormal);
	Vect3 originWorldTorque = CrossProduct(f_w, armNormal);

	// TODO: Ensure that originWorldForce + originWorldTorque equal pt_world - GetPos()
	addForceAtOrigin(originWorldForce);
	addTorqueAtOrigin(originWorldTorque);
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