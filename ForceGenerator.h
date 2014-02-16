#ifndef INDIGO_FROST_FORCE_GENERATOR_H
#define INDIGO_FROST_FORCE_GENERATOR_H

// Pure abstract class, the only reason it's used is to
//  update a force in a given particle for an amount of
//  time.

#include "PhysicsClass.h"
#include "RigidBody.h"

namespace Frost {

	class RigidBodyForceGenerator {
	public:
		virtual void updateForce(RigidBody* rb, float duration) = 0;
	};

}

#endif
