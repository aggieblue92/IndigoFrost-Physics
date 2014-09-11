#ifndef FROSTDLL_API
#ifdef _WINDLL
#define FROSTDLL_API __declspec(dllexport)
#else
#define FROSTDLL_API __declspec(dllimport)
#endif
#endif

/////////////////////////////////////////
// BasicContact - implementation of IContact
//  that does nothing but super basic stuff.
/////////////////////////////////////////

#ifndef FROST_BASIC_CONTACT_H
#define FROST_BASIC_CONTACT_H

#include "IContact.h"
#include "IPhysicsObject.h"

namespace Frost
{
	class BasicContact : public IContact
	{
	public:
		FROSTDLL_API BasicContact(const Vect3& objPt, const Vect3& penetration, IPhysicsObject* receivingObject);

		FROSTDLL_API virtual bool Resolve();

	protected:
		IPhysicsObject* _affectedObject;
	};
}

#endif