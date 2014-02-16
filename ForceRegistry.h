#ifndef INDIGO_FROST_FORCE_REGISTRY_H
#define INDIGO_FROST_FORCE_REGISTRY_H

#include "PhysicsClass.h"
#include "ForceGenerator.h"
#include "RigidBody.h"
#include <vector>

namespace Frost {

	struct RigidBodyForceEntry {
		RigidBodyForceGenerator* gen;
		RigidBody* rigid_body;
		RigidBodyForceEntry(RigidBody* rb, RigidBodyForceGenerator* g)
			: rigid_body(rb), gen(g) {}
	};

	class ForceRegistry {
	public:
		ForceRegistry();
		ForceRegistry(const ForceRegistry& right) {}
		~ForceRegistry() {}

		void Add(RigidBody* rb, RigidBodyForceGenerator* g);
		void Remove(RigidBody* p, RigidBodyForceGenerator* g);
		void ClearRegistry();

		void Shutdown() { ClearRegistry(); }

		void UpdateForces(float timeElapsed);

	protected:
		std::vector<RigidBodyForceEntry*> m_Registry;
	};
}
#endif
