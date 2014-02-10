#include "GravityForce.h"
using namespace Frost;

void GravityForce::updateForce(Particle* p, float duration) {
	if (!p->IsFiniteMass())
		return;

	// Force of gravity = mg
	Vect3 gravityForce(0.0f, 0.0f, 0.0f);
	gravityForce.y = -(p->GetGravity() * p->GetMass());

	// Now, gravity is constant, so we don't care about duration.
	p->AugmentNetForce(gravityForce);
}