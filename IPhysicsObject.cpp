#include "IPhysicsObject.h"
using namespace Frost;

IPhysicsObject::IPhysicsObject()
: Movable()
, _linearVelocity(0.f, 0.f, 0.f)
, _angularVelocity(0.f, 0.f, 0.f)
{}

IPhysicsObject::IPhysicsObject(const IPhysicsObject& toCopy)
: Movable(toCopy)
, _linearVelocity(toCopy._linearVelocity)
, _angularVelocity(toCopy._angularVelocity)
{}

IPhysicsObject::IPhysicsObject(const Movable& inMovable)
: Movable(inMovable)
, _linearVelocity(0.f, 0.f, 0.f)
, _angularVelocity(0.f, 0.f, 0.f)
{}

IPhysicsObject::IPhysicsObject(const FLOAT3& pos, const Quaternion& quat)
: Movable(pos, quat)
, _linearVelocity(0.f, 0.f, 0.f)
, _angularVelocity(0.f, 0.f, 0.f)
{}

IPhysicsObject::IPhysicsObject(const FLOAT3& pos, const Quaternion& orientation, const FLOAT3& linearVelocity, const FLOAT3& angularVelocity)
: Movable(pos, orientation)
, _linearVelocity(linearVelocity)
, _angularVelocity(angularVelocity)
{}