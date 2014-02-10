#ifndef _PARTICLE_COLLISION_BOUNDING_SPHERE_CONTACT_KAM_H_
#define _PARTICLE_COLLISION_BOUNDING_SPHERE_CONTACT_KAM_H_

#include "ParticleLink.h"

namespace Frost {
	class ParticleCollisionSphere : public ParticleLink {
	public:
		ParticleCollisionSphere(Particle* p1, 
			Particle* p2, float len, float elasticity);

		float m_length;
		float m_elasticity;

		virtual unsigned int fillContact(ParticleContact* contact,
			unsigned int limit) const;
	private:

	};
}
#endif
