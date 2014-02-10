#ifndef _PARTICLE_CONTACT_RESOLVER_KAM_H_
#define _PARTICLE_CONTACT_RESOLVER_KAM_H_

#include "ParticleContact.h"

namespace Frost {
	class ParticleContactResolver {
	protected:
		// Number of iterations allowed
		unsigned int m_nIterations;
		unsigned int m_nIterationsUsed;

	public:
		ParticleContactResolver(unsigned int iterations);
		void setIterations(unsigned int iterations);
		void ResolveContacts(ParticleContact* contactList,
			unsigned int numContacts,
			float duration);
	};
}
#endif
