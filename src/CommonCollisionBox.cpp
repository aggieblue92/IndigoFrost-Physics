#include "CommonCollisionBox.h"
#include "CommonContact.h"
using namespace Frost;

CommonCollisionBox::CommonCollisionBox(const FLOAT3& halfSize, const FLOAT3& pos, const Quaternion& orientation, float bounciness, float frictionCoefficient, IPhysicsObject* attachedObject)
: CollisionBox(halfSize, pos, orientation, attachedObject)
, _frictionCoefficient(frictionCoefficient)
, _bouncinessCoefficient(bounciness)
{}

CommonCollisionBox::CommonCollisionBox(const CommonCollisionBox& o)
: CollisionBox(o)
, _frictionCoefficient(o._frictionCoefficient)
, _bouncinessCoefficient(o._bouncinessCoefficient)
{}

IContact* CommonCollisionBox::SummonDemons(const Vect3& pt, const Vect3& pen, IPhysicsObject* thisObj, IPhysicsObject* otherObj) const
{
	return new CommonContact(pt, pen, _bouncinessCoefficient, _frictionCoefficient, thisObj, otherObj);
}