#ifndef FROST_FORCES_ANCHORED_SPRING_FORCE
#define FROST_FORCES_ANCHORED_SPRING_FORCE

#include "IForce.h"

namespace Frost
{
	class AnchoredSpringForce : public IForce
	{
	public:
		AnchoredSpringForce(const Vect3& worldAnchor, const Vect3& localConnectionPoint,
			float springConstant, float restLength);
		AnchoredSpringForce(const AnchoredSpringForce& o);
		virtual void ApplyForce(IPhysicsObject* affected, float timeDuration);
		virtual IForce* getNewForcePtr() const;

	private:
		Vect3 _anchor_world;
		Vect3 _connection_local;
		float _springConstant;
		float _restLength;
	};
}

#endif