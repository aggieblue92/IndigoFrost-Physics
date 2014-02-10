#ifndef _ANCHORED_SPRING_FORCE_KAM_H_
#define _ANCHORED_SPRING_FORCE_KAM_H_

#include "ForceGenerator.h"

//////////////////////////////////////////////////
// CLASS: AnchoredSpring
// DESC: A force generator simulating a spring
//       that is attached to a solid, static
//       point in space.
// REQS: Attachment (or anchor) point (Vect3)
//       Spring Constant (k, in N/m)  (float)
//       Resting Length (in m)        (float)
// NOTE: Use as a ParticleForceGenerator,
//       register with ForceRegistry class
//////////////////////////////////////////////////

namespace Frost {
	class AnchoredSpring : public ParticleForceGenerator {
	public:
		AnchoredSpring(Vect3 anchor, float springConstant,
			float restLength);
		virtual void updateForce(Particle* p, float duration);

	private:
		Vect3 m_anchor;
		float m_springConstant;
		float m_restLength;
	};
}
#endif
