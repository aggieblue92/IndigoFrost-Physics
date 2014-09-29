#include "AnchoredSpringForce.h"
using namespace Frost;

AnchoredSpringForce::AnchoredSpringForce(const Vect3& w, const Vect3& l, float k, float r)
: _anchor_world(w)
, _connection_local(l)
, _springConstant(k)
, _restLength(r)
{}

AnchoredSpringForce::AnchoredSpringForce(const AnchoredSpringForce& o)
: _anchor_world(o._anchor_world)
, _connection_local(o._connection_local)
, _springConstant(o._springConstant)
, _restLength(o._restLength)
{}

void AnchoredSpringForce::ApplyForce(IPhysicsObject* affected, float dt)
{
	Vect3 conn_world = affected->GetTransformMatrix() * _connection_local;
	Vect3 disp = _anchor_world - conn_world;
	float dx = disp.Magnitude() - _restLength;
	affected->addForceAtPoint(Vect3Normal(disp) * dx * _springConstant, conn_world);
}

IForce* AnchoredSpringForce::getNewForcePtr() const
{
	return new AnchoredSpringForce(*this);
}