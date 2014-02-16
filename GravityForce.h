#ifndef INDIGO_FROST_GRAVITY_FORCE_H
#define INDIGO_FROST_GRAVITY_FORCE_H

// Notice: As the engine gets larger, it may be more
//  prudent to implement gravity directly inside the
//  object class, as gravity applies fairly universally.

#include "ForceGenerator.h"

namespace Frost {
	class GravityForce : public RigidBodyForceGenerator {
	public:
		GravityForce() {}
		virtual void updateForce(RigidBody* p, float duration);

	};
}
#endif
