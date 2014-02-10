#include "CollisionBoundingSphereContact.h"
using namespace Frost;

ParticleCollisionSphere::ParticleCollisionSphere(
	Particle* p1, Particle* p2, float len, float elasticity) {

	m_particle[0] = p1;
	m_particle[1] = p2;

	m_length = len;
	m_elasticity = elasticity;
}

unsigned int ParticleCollisionSphere::fillContact(
	ParticleContact* contact,
	unsigned int limit) const {

	float curLen = this->currentLength();

	// Just get out if the length is long
	if (curLen >= this->m_length)
		return 0U;

	// Generate the contact...
	contact->m_particle[0] = m_particle[0];
	contact->m_particle[1] = m_particle[1];

	Vect3 normal = m_particle[0]->GetPosition() -
		m_particle[1]->GetPosition();
	normal.Normalize();

	contact->m_contactNormal = normal;

	contact->m_penetration = m_length - curLen;
	contact->m_restitutionCoefficient = m_elasticity;

	return 1U;
}