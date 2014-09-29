#ifndef FROST_SPRING_FORCE_H
#define FROST_SPRING_FORCE_H

/////////////////////////////////////////
// SpringForce: Connects two objects
//  together via a spring
/////////////////////////////////////////

#include "IPhysicsObject.h"
#include "IForce.h"

namespace Frost
{
	class SpringForce : public IForce
	{
	public:
		SpringForce(const Vect3& localConnectionPoint,
			IPhysicsObject* otherObject,
			const Vect3& otherConnectionPoint,
			float springConstant,
			float restLength);
		SpringForce(const SpringForce& o);

		virtual void ApplyForce(IPhysicsObject* affectedPhysicsObject, float duration);
		virtual IForce* getNewForcePtr() const;

	private:
		IPhysicsObject* _otherObjectInvolved;
		float _springConstant;
		float _restLength;
		Vect3 _localConnectionPoint;
		Vect3 _otherLocalConnectionPoint;
	};
}

#endif