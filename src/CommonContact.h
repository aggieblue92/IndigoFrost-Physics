#ifndef FROST_COMMON_CONTACT_H
#define FROST_COMMON_CONTACT_H

/////////////////////////////////////////
// CommonContact: Implementation of IContact
//  that is used by CommonCollision* objects.
//  Includes friction, bounciness, etc.
/////////////////////////////////////////

#include "IContact.h"
#include "IPhysicsObject.h"

namespace Frost
{
	class CommonContact : public IContact
	{
	public:
		CommonContact(const Vect3& pt, const Vect3& penetration, float bounciness, float friction, IPhysicsObject* receivingObject, IPhysicsObject* otherObject);

		virtual bool Resolve(float dt);

	protected:
		IPhysicsObject* _affectedObject;
		IPhysicsObject* _otherObject;
		float _bounciness;
		float _friction;
	};
}

#endif