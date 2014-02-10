#ifndef _PARTICLE_MAP_WALL_CONTACT_KAM_H_
#define _PARTICLE_MAP_WALL_CONTACT_KAM_H_

#include "ParticleLink.h"

namespace Frost {
	class ParticleMapWallContact : public ParticleLink {
	public:
		ParticleMapWallContact(Particle* p,
			Vect3 bound, Vect3 contactNormal);
		~ParticleMapWallContact();

		virtual unsigned int fillContact(
			ParticleContact* contact,
			unsigned int limit) const;

	private:
		Vect3 m_outerBound;
		Vect3 m_normalDirection;
		Particle* m_boundParticle;
		Particle* m_wallParticle;
	};
}
#endif
