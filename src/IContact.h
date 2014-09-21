/////////////////////////////////////////
// IContact - Base interface for a contact
//  Holds some data about the collision
/////////////////////////////////////////

#ifndef FROST_CONTACT_INTERFACE_H
#define FROST_CONTACT_INTERFACE_H

#include "Movable.h"

namespace Frost
{
	class IContact
	{
	public:
		IContact(const Vect3& objPt, const Vect3& penetrationVector);

		// This method resolves or suggests a resolution between the objects in collision.
		// Returns: True if resolution succeeds, false if not.
		virtual bool Resolve() = 0;

		Vect3 getCollisionPoint() const;
		Vect3Normal getContactNormal() const;
		float getContactMagnitude() const;

	protected:
		Vect3 _objCollisionPoint;

		Vect3Normal _contactNormal;
		float _contactMagnitude;
	};
}

#endif