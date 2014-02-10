#include "ForceRegistry.h"
using namespace Frost;

ForceRegistry::ForceRegistry() {}

void ForceRegistry::Add(Particle* p, ParticleForceGenerator* g) {
	// Register the given particle and force.
	//  Exit w/o fail if either is invalid.
	if (0 == p || 0 == g)
		return;
	else
		m_Registry.push_back(new ParticleForceEntry(p, g));
}

void ForceRegistry::ClearRegistry() {
	// For each value in the registry, take it out.
	for (int i = 0; i < m_Registry.size(); i++) {
		if (0 != m_Registry[i])
			delete m_Registry[i];
	}
	m_Registry.clear();
}

void ForceRegistry::Remove(Particle* p, ParticleForceGenerator* g) {
	for (int i = 0; i < m_Registry.size(); i++) {
		if ((m_Registry[i]->gen == g || 0 == g)
			&&
			(m_Registry[i]->particle == p || 0 == p)) {
			m_Registry.erase(m_Registry.begin() + i);
		}
	}
}

void ForceRegistry::UpdateForces(float timeElapsed) {
	for (int i = 0; i < m_Registry.size(); i++){
		m_Registry[i]->gen->updateForce(m_Registry[i]->particle, timeElapsed);
	}
}