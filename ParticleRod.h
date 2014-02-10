#ifndef _PARTICLE_ROD_KAM_H_
#define _PARTICLE_ROD_KAM_H_

#include "ParticleLink.h"

namespace Frost {
	// Todo: Make a constructor for this, use it instead.

	class ParticleRod : public ParticleLink {
	public:
		float length;
		virtual unsigned int fillContact(
			ParticleContact* contact,
			unsigned int limit) const;
	};
}
#endif