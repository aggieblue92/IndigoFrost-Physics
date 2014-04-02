#include "ForceRegistry.h"
using namespace Frost;

ForceRegistry::ForceRegistry() {}

void ForceRegistry::Add(RigidBody* rb, RigidBodyForceGenerator* g) {
	// Register the given particle and force.
	//  Exit if either is invalid.
	if (0 == rb || 0 == g)
		return;
	else
		m_Registry.push_back(new RigidBodyForceEntry(rb, g));
}

void ForceRegistry::ClearRegistry() {
	// For each value in the registry, take it out.
	for (int i = 0; i < m_Registry.size(); i++) {
		if (0 != m_Registry[i])
			delete m_Registry[i];
	}
	m_Registry.resize(0);
	m_Registry.clear();
}

bool ForceRegistry::Remove(RigidBody* rb, RigidBodyForceGenerator* g) {
	// Search each entry to see if the rigid body is
	//  the one specified (always true if 0 specified),
	//  and same for the force generator.
	bool toReturn = false;
	for (int i = 0; i < m_Registry.size(); i++) {
		if ((m_Registry[i]->gen == g || 0 == g)
			&&
			(m_Registry[i]->rigid_body == rb || 0 == rb)) {
				// Release memory, clear space from vector,
				//  return
				delete m_Registry[i];
				m_Registry.erase(m_Registry.begin() + i);
				toReturn = true;
		}
	}
	return toReturn;
}

void ForceRegistry::UpdateForces(float timeElapsed) {
	// For every force entry, call the updateForce method of the
	//  RigidBodyForceGenerator class on the body, for the amount
	//  of elapsed time (in seconds usually)
	for (int i = 0; i < m_Registry.size(); i++){
		m_Registry[i]->gen->updateForce(m_Registry[i]->rigid_body, timeElapsed);
	}
}