#include "ContactRegistry.h"
using namespace Frost;

ContactRegistry::ContactRegistry() {}

void ContactRegistry::Add(Particle* p, ParticleLink* c) {
	// Register the given contact and particle
	if (0 == p || 0 == c)
		return;
	else
		m_Registry.push_back(new ParticleContactEntry(p, c));
}

void ContactRegistry::Remove(Particle* p, ParticleLink* c) {
	for (int i = 0; i < m_Registry.size(); i++) {
		if ((m_Registry[i]->link == c || 0 == c)
			&&
			(m_Registry[i]->particle == p || 0 == p)) {
			m_Registry.erase(m_Registry.begin() + i);
		}
	}
}

void ContactRegistry::ClearRegistry() {
	for (int i = 0; i < m_Registry.size(); i++) {
		if (0 != m_Registry[i])
			delete m_Registry[i];
	}
	m_Registry.clear();
}

void ContactRegistry::RunContactResolution(float timeElapsed, int iterations) {
	if (m_Registry.size() <= 0)
		return;

	// First off, fill the contact list. Run through all of our
	//  objects in the registry, add them if they need resolution.
	std::vector<ParticleContact*> toResolve(0);

	for (unsigned int i = 0; i < m_Registry.size(); i++) {
		ParticleContact* contact = new ParticleContact;
		if (0 != m_Registry[i]->link->fillContact(contact, 0)) {
			// Success, add it to registry
			toResolve.push_back(contact);
		}
		else {
			// I pity the fool.
			delete contact;
			contact = 0;
		}
	}

	// Default for iterations is a negative value, indicating
	//  use twice as many iterations as there are contacts.
	if (iterations < 0)
		iterations = toResolve.size() * 2;

	for (unsigned int i = 0; i < iterations; i++) {
		// Find most urgent contact, resolve it.
		// Scan modified object, does it need any additional
		//  contact resolution? If so, add it to the contact
		//  list.
		if (toResolve.size() <= 0)
			return;

		int maxContact = -1;
		for (int i = 0; i < toResolve.size(); i++) {
			if (0 == toResolve[i])
				continue;
			if (-1 == maxContact) {
				maxContact = i;
				continue;
			}
			if (toResolve[i]->m_penetration > toResolve[maxContact]->m_penetration)
				maxContact = i;
		}

		// If all of the contacts have been dealt with, no new
		//  ones have arien, just get out of this loop.
		if (-1 == maxContact)
			break;

		// Maximum found, resolve it.
		toResolve[maxContact]->Resolve(timeElapsed);
		delete toResolve[maxContact];
		toResolve[maxContact] = 0;
	}

	// Clean up our list of contacts to resolve.
	for (int i = 0; i < toResolve.size(); i++) {
		if (0 != toResolve[i]) {
			delete toResolve[i];
			toResolve[i] = 0;
		}
	}
	toResolve.clear();
}