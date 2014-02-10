#ifndef _PARTICLE_LINK_CLASS_KAM_H_
#define _PARTICLE_LINK_CLASS_KAM_H_

#include "ParticleContact.h"
#include "PhysicsClass.h"

namespace Frost {
	class ParticleLink {
	public:
		// Holds connected particles...
		Particle* m_particle[2];

	protected:
		float currentLength() const;

	public:
		virtual unsigned int fillContact(
			ParticleContact* contact,
			unsigned int limit) const = 0;
	};
}
#endif
