#include "ParticleContact.h"
using namespace Frost;

ParticleContact::ParticleContact(Particle* firstParticle,
	Particle* secondParticle,
	float restitutionCoefficient,
	Vect3 contactNormal,
	float penetration) :
	m_restitutionCoefficient(restitutionCoefficient),
	m_contactNormal(contactNormal),
	m_penetration(penetration)
{
	m_particle[0] = firstParticle;
	m_particle[1] = secondParticle;
}

ParticleContact::ParticleContact() {
	m_particle[0] = 0;
	m_particle[1] = 0;
	m_restitutionCoefficient = 0.0f;
	m_contactNormal = Vect3(0.0f, 0.0f, 0.0f);
	m_penetration = 0.0f;
}

void ParticleContact::Resolve(float duration) {
	resolveVelocity(duration);
	resolveInterpenetration(duration);
}

float ParticleContact::calculateSeparatingVelocity() const {
	Vect3 relativeVelocity = m_particle[0]->GetVelocity();
	if (m_particle[1])
		relativeVelocity -= m_particle[1]->GetVelocity();
	return relativeVelocity * m_contactNormal;
}

void ParticleContact::resolveVelocity(float duration) {
	// Find velocity in direction of contact...
	float separatingVelocity = calculateSeparatingVelocity();

	// Check whether it needs resolution
	if (separatingVelocity > 0.0f)
		return;

	// Calculate the new separating velocity...
	float newSeparatingVelocity =
		-separatingVelocity * m_restitutionCoefficient;

	// Check the velocity build up due to acceleration
	//  in the last frame only.
	Vect3 accCausedVelocity = m_particle[0]->GetAcceleration();
	if (m_particle[1])
		accCausedVelocity -= m_particle[1]->GetAcceleration();
	float accCausedSepVelocity = accCausedVelocity *
		m_contactNormal * duration;

	// If we've got a closing velocity due to acceleration
	//  build up only, just remove it.
	if (accCausedSepVelocity < 0) {
		newSeparatingVelocity += m_restitutionCoefficient *
			accCausedSepVelocity;

		if (newSeparatingVelocity < 0.0f)
			newSeparatingVelocity = 0.0f;
	}

	// Aaand continue on to find how much we need
	//  to change the velocity by. <- SUCK IT ENGLISH MAJORS.
	// !!! Watch out - is this right? minus a negative?
	float deltaVelocity =
		newSeparatingVelocity - separatingVelocity;

	// Apply change in velocity in proportion to inverse
	//  mass...
	float totalInverseMass = m_particle[0]->GetInverseMass();
	if (m_particle[1])
		totalInverseMass += m_particle[1]->GetInverseMass();

	// If all particles have infiniate mass, impulse has no effect.
	if (totalInverseMass <= 0)
		return;

	// Calculate the impulse to apply
	float impulse = deltaVelocity / totalInverseMass;

	// Find amount of impulse per unit of inverse mass...
	Vect3 impulsePerIMass = m_contactNormal * impulse;

	// Apply impulses...
	m_particle[0]->SetVelocity(m_particle[0]->GetVelocity() +
		impulsePerIMass * m_particle[0]->GetInverseMass());
	if (m_particle[1])
		m_particle[1]->SetVelocity(m_particle[1]->GetVelocity() -
		impulsePerIMass * m_particle[1]->GetInverseMass());
}

void ParticleContact::resolveInterpenetration(float duration) {
	// If there isn't any penetration, why are we here?
	//  'Tis the question.
	if (m_penetration <= 0) 
		return;

	// Move each object based on the inverse mass.
	float totalInverseMass = m_particle[0]->GetInverseMass();
	if (m_particle[1])
		totalInverseMass += m_particle[1]->GetInverseMass();

	// If we're talking two immovable things, get out!
	if (totalInverseMass <= 0)
		return;

	// Find amount of penetration resolution per unit inverse mass..
	Vect3 movePerIMass = m_contactNormal *
		(m_penetration / totalInverseMass);

	// Apply penetration resolution...
	m_particle[0]->SetPosition(m_particle[0]->GetPosition() +
		movePerIMass * m_particle[0]->GetInverseMass());
	if (m_particle[1])
		m_particle[1]->SetPosition(m_particle[1]->GetPosition() -
		movePerIMass * m_particle[1]->GetInverseMass());
}