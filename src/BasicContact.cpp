#include "BasicContact.h"
using namespace Frost;

BasicContact::BasicContact(const Vect3& objPt, const Vect3& penetration, IPhysicsObject* receivingObject)
: IContact(objPt, penetration)
, _affectedObject(receivingObject)
{}

bool BasicContact::Resolve()
{
	// If there is no attached object, GTFO
	if (_affectedObject == 0) return true;

	// Otherwise, handle it using the super simple
	//  spring formula with a pretty rigid string.
	// F = -kx

	const float K = 350.f; // 350 N/m

	_affectedObject->AddForceAtPoint(
		this->_contactNormal * this->_contactMagnitude * K * -1.f,
		this->_objCollisionPoint);

	return true;
}