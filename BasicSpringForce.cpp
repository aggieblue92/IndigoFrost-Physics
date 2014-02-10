#include "BasicSpringForce.h"
using namespace Frost;

BasicSpringForce::BasicSpringForce(Particle* anchor,
	float springConstant, float restLength) :
	m_anchor(anchor),
	m_springConstant(springConstant),
	m_restLength(restLength)
{}

void BasicSpringForce::updateForce(Particle* p, float duration) {
	Vect3 force;

	// Calculate vector of the spring
	force = p->GetPosition();
	force -= m_anchor->GetPosition();

	// Calculate magnitude of the force
	float magnitude = force.Magnitude();
	magnitude = (magnitude - m_restLength);
	magnitude *= m_springConstant;

	// Calculate final force and apply it.
	force.Normalize();
	force *= -magnitude;
	p->AugmentNetForce(force);
}