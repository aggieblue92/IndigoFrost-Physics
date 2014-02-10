#ifndef _PARTICLE_CONTACT_KAM_H_
#define _PARTICLE_CONTACT_KAM_H_

#include "PhysicsClass.h"

namespace Frost {
	class ParticleContact {
	public:
		ParticleContact();
		ParticleContact(Particle* firstParticle,
			Particle* secondParticle,
			float restitutionCoefficient,
			Vect3 contactNormal,
			float penetration);

		// Holds particles that are involved in the contact.
		//  Second one is NULL for scenery contacts.
		//  Contact is from perspective of the first.
		Particle* m_particle[2];
		float m_restitutionCoefficient;
		Vect3 m_contactNormal;
		float m_penetration;

		void Resolve(float duration);
		float calculateSeparatingVelocity() const;

	private:
		void resolveVelocity(float duration);
		void resolveInterpenetration(float duration);
	};
}
#endif
