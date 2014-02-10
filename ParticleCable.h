#ifndef _PARTICLE_CABLE_KAM_H_
#define _PARTICLE_CABLE_KAM_H_

#include "ParticleLink.h"

namespace Frost {
	class ParticleCable : public ParticleLink {
	public:
		ParticleCable(Particle* p1, Particle* p2,
			float maxLength, float bounciness);

		float m_maxLength;
		float m_bounciness;

		virtual unsigned int fillContact(
			ParticleContact* contact,
			unsigned int limit) const;
	};
}
#endif
