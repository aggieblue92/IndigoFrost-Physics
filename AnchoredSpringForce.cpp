#include "AnchoredSpringForce.h"
using namespace Frost;

AnchoredSpring::AnchoredSpring(Vect3 anchor, float springConstant,
	float restLength) :
	m_anchor(anchor),
	m_springConstant(springConstant),
	m_restLength(restLength)
{}

void AnchoredSpring::updateForce(Particle* p, float duration) {
	Vect3 force;
	
	// Calculate vector of the spring
	force = p->GetPosition();
	force -= m_anchor;

	// Calculate the magnitude of the force
	float magnitude = force.Magnitude();
	magnitude = (magnitude - m_restLength);
	magnitude *= m_springConstant;

	// Calculate final force and apply it.
	force.Normalize();
	force *= -magnitude;
	p->AugmentNetForce(force);
}