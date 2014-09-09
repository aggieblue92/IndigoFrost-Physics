#include "IContact.h"
using namespace Frost;

IContact::IContact(const Vect3& objPt, const Vect3& penetrationVector)
: _objCollisionPoint(obj1Pt)
, _contactNormal(penetrationVector)
, _contactMagnitude(penetrationVector.Magnitude())
{}