#include "GravityForce.h"
using namespace Frost;

void GravityForce::updateForce(RigidBody* rb, float duration) {
	if (!rb->IsFiniteMass())
		return;

	// Force of gravity = mg
	Vect3 gravityForce(0.0f, 0.0f, 0.0f);
	gravityForce.y = -(rb->getGravity() * rb->GetMass());

	// Now, gravity is constant, so we don't care about duration.
	rb->addForce(gravityForce);
}