#include "ParticleWallContact.h"
using namespace Frost;

ParticleWallContact::ParticleWallContact(Vect3 * origin, Vect3 up, Vect3 side) :
	m_wallOrigin(origin), m_wallUp(up), m_wallSide(side) {}

ParticleWallContact::~ParticleWallContact() {}

unsigned int ParticleWallContact::fillContact(
	ParticleContact* contact,
	unsigned int limit) const {

	// Insert black magic here.

	// ...

	// Cross products AND dot products AND rotation matrices....

	// Insert virgin sacrifices.

	return 0U;
}