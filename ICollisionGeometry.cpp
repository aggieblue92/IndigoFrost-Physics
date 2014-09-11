#include "ICollisionGeometry.h"
using namespace Frost;

ICollisionGeometry::ICollisionGeometry(FROST_COLLISION_GEOMETRY_TYPE type)
: _type(type)
, _attachedObject(0)
{}

ICollisionGeometry::ICollisionGeometry(FROST_COLLISION_GEOMETRY_TYPE type, const FLOAT3& pos, const Quaternion& orientation)
: Movable(pos, orientation)
, _type(type)
, _attachedObject(0)
{}

ICollisionGeometry::ICollisionGeometry(FROST_COLLISION_GEOMETRY_TYPE type, const FLOAT3& pos, const Quaternion& orientation, IPhysicsObject* attached)
: Movable(pos, orientation)
, _type(type)
, _attachedObject(attached)
{}

ICollisionGeometry::ICollisionGeometry(const ICollisionGeometry& copy)
: Movable(copy)
, _type(copy._type)
, _attachedObject(copy._attachedObject)
{}

FROST_COLLISION_GEOMETRY_TYPE ICollisionGeometry::GetType()
{
	return _type;
}

IPhysicsObject* ICollisionGeometry::GetAttachedObjectPtr() const
{
	return _attachedObject;
}