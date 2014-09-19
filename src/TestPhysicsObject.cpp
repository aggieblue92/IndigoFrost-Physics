#include "TestPhysicsObject.h"
using namespace Frost;

TestPhysicsObject::TestPhysicsObject(float invMass, const Matrix& invInertiaTensor)
: IPhysicsObject()
, _invMass(invMass)
, _invInertiaTensor(invInertiaTensor)
{}

TestPhysicsObject::TestPhysicsObject(const TestPhysicsObject& o)
: IPhysicsObject(o)
, _invMass(o._invMass)
, _invInertiaTensor(o._invInertiaTensor)
{}

TestPhysicsObject::TestPhysicsObject(float invMass, const Matrix& invInertiaTensor, const FLOAT3& pos, const Quaternion& orientation, const FLOAT3& linearVelocity, const FLOAT3& angularVelocity)
: IPhysicsObject(pos, orientation, linearVelocity, angularVelocity)
, _invMass(invMass)
, _invInertiaTensor(invInertiaTensor)
{}

// Adds a force to the object at the origin point (no angular acc. component)
void TestPhysicsObject::AddForceAtOrigin(const Vect3& forceToAdd_w)
{
	// TODO: Make sure we want to use WORLD forces.
	this->_netForce += forceToAdd_w;
}

// Adds a force to the object at a non-origin point
void TestPhysicsObject::AddForceAtPoint(const Vect3& forceToAdd_w, const Vect3& pt_world)
{
	// Break up force into parallel and perpendicular components...
	Vect3Normal armNormal = pt_world - this->GetPos();
	Vect3 originWorldForce = armNormal * (forceToAdd_w * armNormal);
	Vect3 originWorldTorque = CrossProduct(forceToAdd_w, armNormal);

	// TODO: Test that the force and torque take all the
	//  force, but don't go over anything...
	AddForceAtOrigin(originWorldForce);
	AddTorqueAtOrigin(originWorldTorque);
}

// Adds a torque to the object at the origin point.
void TestPhysicsObject::AddTorqueAtOrigin(const Vect3& torqueToAdd_w)
{
	this->_netTorque += torqueToAdd_w;
}

// Updates the object by clearing all accumulated forces and
//  applying them to the momentum of the object.
void TestPhysicsObject::Update(float timeElapsed)
{
	Vect3 frameAcceleration = _netForce * _invMass;
	Vect3 frameTorque = _invInertiaTensor * _netTorque;

	_linearVelocity += frameAcceleration * timeElapsed;
	_angularVelocity += frameTorque * timeElapsed;

	_netForce = MathConstants::VECTOR_ZERO;
	_netTorque = MathConstants::VECTOR_ZERO;

	SetPos(GetPos() + (_linearVelocity * timeElapsed));
	SetOrientation(GetOrientation() + (_angularVelocity * timeElapsed));

	// Update derived data here, when you actually have it...
}