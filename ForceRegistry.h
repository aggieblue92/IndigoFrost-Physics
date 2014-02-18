#ifndef INDIGO_FROST_FORCE_REGISTRY_H
#define INDIGO_FROST_FORCE_REGISTRY_H

/*****************************************************\

		ForceRegistry.h

Contains:
-RigidBodyForceEntry
-ForceRegistry

	Contains the data structures for holding a complete
set of all forces in a closed environment. Objects of
type RigidBodyForceEntry contain a force generator and
a rigid body, and the ForceRegistry class essentially
just carries a list of such entries, and on calling the
updateForces method, calls the force generator's
updateForce method on the rigid body, for the time elapsed
since the last update.

\*****************************************************/

#include "Vect3.h"
#include "ForceGenerator.h"
#include "RigidBody.h"
#include <vector>

namespace Frost {

	// RigidBodyForceEntry
	// Members:
	// ---gen: A force generator which applies to the rigid body.
	// ---rigid_body: A rigid body to which to apply the force 'gen'
	struct RigidBodyForceEntry {
		RigidBodyForceGenerator* gen;
		RigidBody* rigid_body;
		RigidBodyForceEntry(RigidBody* rb, RigidBodyForceGenerator* g)
			: rigid_body(rb), gen(g) {}
	};

	// ForceRegistry: Contains a registry of all forces and their
	//  respective rigid bodies in a closed environment.
	// NOTE: some forces will have many ForceEntries, especially
	//  very common forces like Gravity. There will probably be
	//  a separate entry for Gravity for every single particle.
	// NOTE: A ForceRegistry object must be cleaned up to release
	//  memory allocated in run-time using the Shutdown() function.
	class ForceRegistry {
	public:
		ForceRegistry();
		ForceRegistry(const ForceRegistry& right) {}
		~ForceRegistry() {}

		// Add a force/body pair to the registry
		void Add(RigidBody* rb, RigidBodyForceGenerator* g);

		// Remove a force/body pair from the registry,
		//  if it exists. Setting rb to zero will remove all
		//  instances of the force, and setting g to zero will
		//  remove all forces from the rigid body.
		// NOTE: Setting both parameters to zero clears the
		//  force registry entirely. So, be careful.
		void Remove(RigidBody* rb, RigidBodyForceGenerator* g);

		// Empty all values from the registry.
		void ClearRegistry();

		// Cleanup the registry. This cleans up memory used.
		void Shutdown() { ClearRegistry(); }

		// Update all force-body pairs.
		// --timeElapsed: time since the last update (usually 1 frame)
		void UpdateForces(float timeElapsed);

	protected:
		// m_Registry: a std::vector containing all the force entries.
		std::vector<RigidBodyForceEntry*> m_Registry;
	};
}
#endif
