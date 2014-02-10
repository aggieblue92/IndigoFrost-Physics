#ifndef _BASIC_SPRING_FORCE_KAM_H_
#define _BASIC_SPRING_FORCE_KAM_H_

#include "ForceGenerator.h"

///////////////////////////////////////////////////////
// CLASS: BasicSpringForce
// DESC: A force generator that ties together two
//       particles in 3D space.
// REQS: Anchor: a particle to which the spring is
//          anchored (Particle)
//       Spring Constant in N/m (float)
//       Resting Length in m (float)
// NOTE: This only applies force (in updateForce)
//       to the particle supplied in updateForce,
//       so the anchor is unaffected.
///////////////////////////////////////////////////////

namespace Frost {
	class BasicSpringForce : public ParticleForceGenerator {
	public:
		BasicSpringForce(Particle* anchor,
			float springConstant, float restLength);
		virtual void updateForce(Particle* p, float duration);

	private:
		Particle* m_anchor;
		float m_springConstant;
		float m_restLength;
	};
}
#endif