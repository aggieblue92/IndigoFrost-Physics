#include "ParticleCable.h"
using namespace Frost;

ParticleCable::ParticleCable(Particle* p1, Particle* p2,
	float maxLength, float bounciness) :
	m_maxLength(maxLength),
	m_bounciness(bounciness) {
	m_particle[0] = p1;
	m_particle[1] = p2;
}

unsigned int ParticleCable::fillContact(
	ParticleContact* contact,
	unsigned int limit) const {

	// Current cable length
	float length = currentLength();

	// Are we overextended?
	if (length < m_maxLength)
		return 0;

	// Fill data...
	contact->m_particle[0] = m_particle[0];
	contact->m_particle[1] = m_particle[1];

	// Calculate the normal
	Vect3 normal = m_particle[1]->GetPosition()
		- m_particle[0]->GetPosition();
	normal.Normalize();
	contact->m_contactNormal = normal;

	contact->m_penetration = length - m_maxLength;
	contact->m_restitutionCoefficient = m_bounciness;

	return 1;
}