#ifndef INDIGO_FROST_GRAVITY_FORCE_H
#define INDIGO_FROST_GRAVITY_FORCE_H

/*****************************************************\

		GravityForce: Applies force due to gravity
	to an object.

NOTE: As the engine gets larger, it may be more prudent
  to include the gravity force directly in integration.
NOTE: Another alternative is to make the acceleration
  due to gravity a member of this force instead of
  a member of the RigidBody class. That actually makes
  a lot of sense, especially for simulating non-gravity
  objects.

\*****************************************************/

#include "ForceGenerator.h"

namespace Frost {
	class GravityForce : public RigidBodyForceGenerator {
	public:
		GravityForce() {}
		virtual void updateForce(RigidBody* p, float duration);

	};
}
#endif
