#ifndef FROSTDLL_API
#ifdef _WINDLL
#define FROSTDLL_API __declspec(dllexport)
#else
#define FROSTDLL_API __declspec(dllimport)
#endif
#endif

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
		FROSTDLL_API IContact(const Vect3& objPt, const Vect3& penetrationVector);

		// This method resolves or suggests a resolution between the objects in collision.
		// Returns: True if resolution succeeds, false if not.
		FROSTDLL_API virtual bool Resolve() = 0;

		FROSTDLL_API Vect3 getCollisionPoint() const;
		FROSTDLL_API Vect3Normal getContactNormal() const;
		FROSTDLL_API float getContactMagnitude() const;

	protected:
		Vect3 _objCollisionPoint;

		Vect3Normal _contactNormal;
		float _contactMagnitude;
	};
}

#endif