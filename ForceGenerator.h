#ifndef _FROST_FORCE_GENERATOR_H_
#define _FROST_FORCE_GENERATOR_H_

// Pure abstract class, the only reason it's used is to
//  update a force in a given particle for an amount of
//  time.

#include "PhysicsClass.h"

namespace Frost {

	class ParticleForceGenerator {
	public:
		virtual void updateForce(Frost::Particle* p, float duration) = 0;
	};

}

#endif
