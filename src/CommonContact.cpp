#include "CommonContact.h"
using namespace Frost;

CommonContact::CommonContact(const Vect3& pt, const Vect3& penetration, float bounciness, float friction, IPhysicsObject* attachedObj)
: IContact(pt, penetration)
, _affectedObject(attachedObj)
, _friction(friction)
, _bounciness(bounciness)
{}

bool CommonContact::Resolve()
{
	throw NotImplementedException();
}