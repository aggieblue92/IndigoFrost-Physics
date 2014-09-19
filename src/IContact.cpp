#include "IContact.h"
using namespace Frost;

IContact::IContact(const Vect3& objPt, const Vect3& penetrationVector)
: _objCollisionPoint(objPt)
, _contactNormal(penetrationVector)
, _contactMagnitude(penetrationVector.Magnitude())
{}

Vect3 IContact::getCollisionPoint() const
{
	return this->_objCollisionPoint;
}

Vect3Normal IContact::getContactNormal() const
{
	return this->_contactNormal;
}

float IContact::getContactMagnitude() const
{
	return this->_contactMagnitude;
}