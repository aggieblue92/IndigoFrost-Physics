#include "SpringForce.h"
using namespace Frost;

SpringForce::SpringForce(const Vect3& lcp,
	IPhysicsObject* o,
	const Vect3& ocp,
	float k,
	float x)
	: _otherLocalConnectionPoint(ocp)
	, _restLength(x)
	, _springConstant(k)
	, _otherObjectInvolved(o)
	, _localConnectionPoint(lcp)
{
	if (o == 0)
	{
		throw NullObjectException();
	}
}

void SpringForce::ApplyForce(IPhysicsObject* me, float dt)
{
	// Calculate the two ends in world space
	Vect3 lws = me->GetTransformMatrix() * _localConnectionPoint;
	Vect3 rhs = _otherObjectInvolved->GetTransformMatrix() * _otherLocalConnectionPoint;

	// Calculate the vector of the spring...
	Vect3 distance = lws - rhs;

	// Calculate the magnitude of the force...
	float magnitude = (distance.Magnitude() - _restLength) * _springConstant;

	// Calculate the final force and apply it
	me->addForceAtPoint(Vect3Normal(distance) * magnitude, lws);
	_otherObjectInvolved->addForceAtPoint(Vect3Normal(distance) * -magnitude, rhs);
}