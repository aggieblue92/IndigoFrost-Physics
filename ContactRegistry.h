#ifndef _CONTACT_REGISTRY_KAM_H_
#define _CONTACT_REGISTRY_KAM_H_

#include "ParticleLink.h"
#include "ParticleContact.h"
#include "PhysicsClass.h"
#include "ParticleContactResolver.h"
#include <vector>

//////////////////////////////////////////////////////////
// CLASS: ContactRegistry
// DESC: Used for processing individual contacts in the
//       world. All contact points are registered in this
//       class, and running the RunContactResolution
//       method handles any contacts seen in the world.
// REQS: Input a list of particles and corresponding links,
//       they will all be processed automatically.
// NOTE: This class runs a finite number of times, as
//       there are definitely occasions in which resolving
//       one contact creates another one. It runs them in
//       order of largest interpenetration distance.
//////////////////////////////////////////////////////////

namespace Frost {
	struct ParticleContactEntry {
		ParticleLink* link;
		Particle* particle;
		ParticleContactEntry(Particle* p, ParticleLink* l) :
			link(l), particle(p) {}
	};

	class ContactRegistry {
	public:
		ContactRegistry();
		ContactRegistry(const ContactRegistry& right) {}
		~ContactRegistry() {}

		void Add(Particle* p, ParticleLink* c);
		void Remove(Particle* p, ParticleLink* c);
		void ClearRegistry();

		void Shutdown() { ClearRegistry(); }

		void RunContactResolution(float timeElapsed, int iterations = -1);

	protected:
		std::vector<ParticleContactEntry*> m_Registry;

	};
}
#endif
