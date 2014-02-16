#ifndef INDIGO_FROST_ANCHORED_SPRING_FORCE_H
#define INDIGO_FROST_ANCHORED_SPRING_FORCE_H

#include "ForceGenerator.h"

//////////////////////////////////////////////////
// CLASS: AnchoredSpring
// DESC: A force generator simulating a spring
//       that is attached to a solid, static
//       point in space.
// REQS: Attachment (or anchor) point (Vect3)
//       Spring Constant (k, in N/m)  (float)
//       Resting Length (in m)        (float)
// NOTE: Use as a RigidBodyForceGenerator,
//       register with ForceRegistry class
//////////////////////////////////////////////////

namespace Frost {
	class AnchoredSpring : public RigidBodyForceGenerator {
	public:
		AnchoredSpring(Vect3 anchor, Vect3 connectionPoint_local,
			float springConstant,
			float restLength);
		virtual void updateForce(RigidBody* rb, float duration);

	private:
		Vect3 m_anchor;
		Vect3 m_connectionPoint; // In local coordinates on the body
		float m_springConstant;
		float m_restLength;
	};
}
#endif
