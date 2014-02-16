#include "ExplosionForce.h"
using namespace Frost;

ExplosionForce::ExplosionForce()
	: m_duration(0.f), m_explosionPower(0.f), m_sourcePos(Vect3()), m_timeRemaining(0.f)
{}

ExplosionForce::ExplosionForce(float duration, float force_per_second, Vect3 sourcePos)
	: m_duration(duration), m_explosionPower(force_per_second), m_sourcePos(sourcePos), m_timeRemaining(0.f)
{}

void ExplosionForce::updateForce(RigidBody* rb, float duration) {
	// If this is inactive, just don't do it.
	if(m_timeRemaining < 0.f)
		return;

	m_timeRemaining -= duration;

	// Finite mass?
	if(!rb->IsFiniteMass())
		return;

	// Get the unit direction we're looking at.
	Vect3 dir = (rb->m_position - m_sourcePos).GetNormal();

	// Multiply by force to add (which is force per second times duration)
	dir *= m_explosionPower * duration;

	// Divide by radius squared? Sure. Why not.
	float radius = (rb->m_position - m_sourcePos).Magnitude();
	dir *= 1 / (radius * radius);

	// Now we have our force.
	rb->addForce(dir);
}

void ExplosionForce::setDuration(float duration) {
	if(duration >= 0.0f)
		m_duration = duration;
}

void ExplosionForce::setForcePerSecond(float force_per_second) {
	// Ooh, a negative force_per_second is an IMPLOSION fool!
	m_explosionPower = force_per_second;
}

void ExplosionForce::setSourcePosition(Vect3 source) {
	m_sourcePos = source;
}

void ExplosionForce::Activate() {
	m_timeRemaining = m_duration;
}

float ExplosionForce::getDuration() {
	return m_duration;
}

float ExplosionForce::getTimeRemaining() {
	return m_timeRemaining;
}

float ExplosionForce::getForcePerSecond() {
	return m_explosionPower;
}

Vect3 ExplosionForce::getSourcePosition() {
	return m_sourcePos;
}

bool ExplosionForce::isActive() {
	return m_timeRemaining > 0.f;
}