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
		BasicContact(const Vect3& objPt, const Vect3& penetration, IPhysicsObject* receivingObject);

		virtual bool Resolve(float dt);

	protected:
		IPhysicsObject* _affectedObject;
	};
}

#endif