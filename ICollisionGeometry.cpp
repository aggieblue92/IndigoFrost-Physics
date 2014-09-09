#include "ICollisionGeometry.h"
using namespace Frost;

ICollisionGeometry::ICollisionGeometry(FROST_COLLISION_GEOMETRY_TYPE type)
: _type(type)
{}

ICollisionGeometry::ICollisionGeometry(FROST_COLLISION_GEOMETRY_TYPE type, const FLOAT3& pos, const Quaternion& orientation)
: Movable(pos, orientation)
, _type(type)
{}

ICollisionGeometry::ICollisionGeometry(const ICollisionGeometry& copy)
: Movable(copy)
, _type(copy._type)
{}

FROST_COLLISION_GEOMETRY_TYPE ICollisionGeometry::GetType()
{
	return _type;
}