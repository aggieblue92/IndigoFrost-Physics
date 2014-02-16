#include "ParticleMapWallContact.h"
using namespace Frost;

ParticleMapWallContact::ParticleMapWallContact
	(Particle* p, Vect3 bound, Vect3 contactNormal) :
	m_boundParticle(p),
	m_outerBound(bound),
	m_normalDirection(contactNormal),
	m_wallParticle(0) {
	
	m_normalDirection.Normalize();
	m_wallParticle = new Particle(
		Vect3(m_outerBound),
		Vect3(),
		Vect3());
	m_wallParticle->SetInverseMass(0.0f);
	m_wallParticle->SetGravity(0.0f);
	m_wallParticle->SetDamping(0.02f);
}

ParticleMapWallContact::~ParticleMapWallContact() {
	if (0 != m_wallParticle) {
		delete m_wallParticle;
		m_wallParticle = 0;
	}
}

unsigned int ParticleMapWallContact::fillContact(
	ParticleContact* contact,
	unsigned int limit) const {

	// Look at bound and direction. If the particle has
	//  gone outside of the bound in the opposite direction
	//  as the normalDirection, then generate a contact
	//  to the outer bound position in the direction
	//  of the contact normal.
	Vect3 relativePosition =
		m_boundParticle->GetPosition() - m_outerBound;

	// Get the magnitude of the adjusted position in the direction
	//  of the contact normal.
	float distanceFromWall = m_normalDirection * relativePosition;
	if (distanceFromWall > 0)
		return 0;
	
	contact->m_particle[0] = m_boundParticle;
	contact->m_particle[1] = m_wallParticle;
	contact->m_contactNormal = m_normalDirection;
	contact->m_penetration = -distanceFromWall;
	contact->m_restitutionCoefficient = 0.08f;
	// We don't want to bounce off of our walls, just thud.
	//  TODO: Change this - make the map walls way bouncy if you want.
}