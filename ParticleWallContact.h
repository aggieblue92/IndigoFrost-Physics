#ifndef _PARTICLE_WALL_CONTACT_HOPEFULLY_
#define _PARTICLE_WALL_CONTACT_HOPEFULLY_

#include "ParticleLink.h"

namespace Frost {
	// Particle Wall Contact:
	//  Set a bounded square contact area that will rebound
	// a particle when it attempts to pass through. Uses
	// black magic and virgin sacrifices.
	class ParticleWallContact : public ParticleLink {
	public:
		ParticleWallContact(Vect3* origin,
			Vect3 up, Vect3 side);
		~ParticleWallContact();

		virtual unsigned int fillContact(
			ParticleContact* contact,
			unsigned int limit) const;

	private:
		Vect3* m_wallOrigin;  // DANGER DANGER DANGER not un-allocated - must be managed externally.
		Vect3 m_wallUp;
		Vect3 m_wallSide;
	};
}

#endif
