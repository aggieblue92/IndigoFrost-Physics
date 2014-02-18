#include "AnchoredSpringForce.h"
using namespace Frost;

AnchoredSpring::AnchoredSpring(Vect3 anchor, Vect3 connectionPoint_local,
			float springConstant,
			float restLength)
			: m_anchor(anchor),
			m_connectionPoint(connectionPoint_local),
			m_springConstant(springConstant),
			m_restLength(restLength)
{}

void AnchoredSpring::updateForce(RigidBody* rb, float duration) {
	// Calculate the connected end in world space...
	Vect3 connection_world = rb->getPointInWorldSpace(m_connectionPoint);

	// Calculate the spring vector (i.e., where does it point?)
	Vect3 force = connection_world - m_anchor;
	
	// Calculate the magnitude of the force...
	float magnitude = force.Magnitude();
	magnitude = magnitude - m_restLength;
	magnitude *= m_springConstant;

	// Calculate the final force and apply it
	force.Normalize();
	force *= -magnitude; // Note def. of force
	// Note - this is modified. Not sure it's right.
	rb->addForceAtPoint(force, connection_world);
}