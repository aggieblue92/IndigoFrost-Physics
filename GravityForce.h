#ifndef _GRAVITY_FORCE_KAM_H_
#define _GRAVITY_FORCE_KAM_H_

// Notice: As the engine gets larger, it may be more
//  prudent to implement gravity directly inside the
//  object class, as gravity applies fairly universally.

#include "ForceGenerator.h"

namespace Frost {
	class GravityForce : public ParticleForceGenerator {
	public:
		GravityForce() {}
		virtual void updateForce(Particle* p, float duration);
	};
}
#endif
