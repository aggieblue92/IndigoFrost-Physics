#include "CommonCollisionSphere.h"
#include "CommonContact.h"
using namespace Frost;

CommonCollisionSphere::CommonCollisionSphere(float radius, const FLOAT3& pos, float bounciness, float friction, IPhysicsObject* attachedObject)
: CollisionSphere(radius, pos, attachedObject)
, _frictionCoefficient(friction)
, _bouncinessCoefficient(bounciness)
{}

CommonCollisionSphere::CommonCollisionSphere(const CommonCollisionSphere& o)
: CollisionSphere(o)
, _frictionCoefficient(o._frictionCoefficient)
, _bouncinessCoefficient(o._bouncinessCoefficient)
{}

IContact* CommonCollisionSphere::SummonDemons(const Vect3& pt_w, const Vect3& pen, IPhysicsObject* otherObj)
{
	return new CommonContact(pt_w, pen, _bouncinessCoefficient, _frictionCoefficient, otherObj);
}