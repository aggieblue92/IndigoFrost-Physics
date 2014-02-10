#include "ParticleRod.h"
using namespace Frost;

unsigned int ParticleRod::fillContact(
	ParticleContact* contact,
	unsigned int limit) const {

	float currentLength = this->currentLength();

	if (currentLength == length)
		return 0;

	// Return the contact...
	contact->m_particle[0] = m_particle[0];
	contact->m_particle[1] = m_particle[1];

	Vect3 normal = m_particle[1]->GetPosition() -
		m_particle[0]->GetPosition();
	normal.Normalize();

	// Contact depends on whether we're extending
	//  or compressing.
	if (currentLength > length) {
		contact->m_contactNormal = normal;
		contact->m_penetration =
			currentLength - length;
	}
	else {
		contact->m_contactNormal = normal * -1.0f;
		contact->m_penetration = length - currentLength;
	}

	// No bounciness on our rod. That would be a spring.
	contact->m_restitutionCoefficient = 0;

	return 1;
}