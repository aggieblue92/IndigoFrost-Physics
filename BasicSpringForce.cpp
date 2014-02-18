#include "BasicSpringForce.h"
using namespace Frost;

BasicSpringForce::BasicSpringForce(const Vect3& localConnectionPt,
			RigidBody* other,
			const Vect3& otherConnectionPt,
			float springConstant,
			float restLength)
			: m_connectionPoint(localConnectionPt),
			m_otherConnectionPoint(otherConnectionPt),
			m_other(other),
			m_springConstant(springConstant),
			m_restLength(restLength)
{}

void BasicSpringForce::updateForce(RigidBody* rb, float duration) {
	// Calculate the two ends in world space
	Vect3 lws = rb->getPointInWorldSpace(m_connectionPoint);
	Vect3 rhs = m_other->getPointInWorldSpace(m_otherConnectionPoint);

	// Calculate the vector of the spring...
	Vect3 force = lws - rhs;

	// Calculate the magnitude of the force...
	float magnitude = force.Magnitude();
	magnitude = (magnitude - m_restLength);
	magnitude *= m_springConstant;

	// Calculate the final force and apply it
	force.Normalize();
	force *= -magnitude; // Note definition of force.
	rb->addForceAtPoint(force, lws);
}