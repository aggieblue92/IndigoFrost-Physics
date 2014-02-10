#ifndef _FORCE_REGISTRY_KAM_H_
#define _FORCE_REGISTRY_KAM_H_

#include "PhysicsClass.h"
#include "ForceGenerator.h"
#include <vector>

namespace Frost {
	struct ParticleForceEntry {
		ParticleForceGenerator* gen;
		Particle* particle;
		ParticleForceEntry(Particle* p, ParticleForceGenerator* g) :
			particle(p), gen(g) {}
	};

	class ForceRegistry {
	public:
		ForceRegistry();
		ForceRegistry(const ForceRegistry& right) {}
		~ForceRegistry() {}

		void Add(Particle* p, ParticleForceGenerator* g);
		void Remove(Particle* p, ParticleForceGenerator* g);
		void ClearRegistry();

		void Shutdown() { ClearRegistry(); }

		void UpdateForces(float timeElapsed);

	protected:
		std::vector<ParticleForceEntry*> m_Registry;
	};
}
#endif
