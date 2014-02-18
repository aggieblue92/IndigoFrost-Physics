#ifndef INDIGO_FROST_FORCE_GENERATOR_H
#define INDIGO_FROST_FORCE_GENERATOR_H

/*****************************************************\

    Rigid Body Force Generator class

Purpose: Easily plug into ForceRegistry as an abstract
  force class. The only method guaranteed common is the
  'updateForce' function, which adds the amount of
  force produced to the rigid body.

Other details (application location, magnitude, etc.)
  are up to the derived base classes.

\*****************************************************/

#include "Vect3.h"
#include "RigidBody.h"

namespace Frost {

	class RigidBodyForceGenerator {
	public:
		// Apply force to rigid body
		// --- rb: Rigid Body to which to apply the force.
		// --- duration: Time since last update (usually 1 frame)
		virtual void updateForce(RigidBody* rb, float duration) = 0;
	};

}

#endif
