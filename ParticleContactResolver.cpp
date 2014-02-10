#include "ParticleContactResolver.h"
using namespace Frost;

ParticleContactResolver::ParticleContactResolver(unsigned int num) {
	m_nIterations = num;
	m_nIterationsUsed = 0;
}

void ParticleContactResolver::setIterations(unsigned int iterations) {
	m_nIterations = iterations;
	m_nIterationsUsed = 0;
}

void ParticleContactResolver::ResolveContacts(
	ParticleContact* contactList,
	unsigned int numContacts,
	float duration) {

	m_nIterationsUsed = 0;
	// TODO: Can optimize this bit by keeping
	//  a sorted tree or something of these
	//  suckers instead of manually selecting
	//  every time as such.

	// ALSO: It's been heavily modified...
	/*
	while (m_nIterationsUsed < m_nIterations) {
		float max = 0.0f;
		unsigned int maxIndex = numContacts;

		for (unsigned int i = 0; i < numContacts; i++) {
			float sepVal = contactList[i].calculateSeparatingVelocity();
			if (sepVal < max) {
				max = sepVal;
				maxIndex = i;
			}
		}

		// Resolve this contact
		contactList[maxIndex].Resolve(duration);
		m_nIterationsUsed++;
	}*/
	for (int i = 0; i < numContacts; i++) {
		contactList[i].Resolve(duration);
	}
}