#include "PhysicsClass.h"
using namespace Frost;

Particle::Particle() {
	// Set null for everything except 10 m/s for gravity
	//  and a damping of 0.93, mass of 1kg
	m_g = 10.0f;
	m_damp = 0.93f;
	m_inverseMass = 1.0f;

	m_acc = Vect3(0.0f, 0.0f, 0.0f);
	m_vel = Vect3(0.0f, 0.0f, 0.0f);
	m_acc = Vect3(0.0f, -0.0f, 0.0f);

	// Now, the programmer never really gets to touch this directly.
	m_netForces = Vect3(0.0f, 0.0f, 0.0f);
}

Particle::Particle(const Particle& right) :
	m_acc(right.m_acc),
	m_vel(right.m_vel),
	m_pos(right.m_pos),
	m_damp(right.m_damp),
	m_inverseMass(right.m_inverseMass),
	m_g(right.m_g),
	m_netForces(right.m_netForces)
{}

Particle::Particle(Vect3 pos, Vect3 vel,
	Vect3 acc) :
	m_pos(pos), m_vel(vel), m_acc(acc),
	m_inverseMass(1.0f), m_g(1.0f),
	m_damp(0.5f)
{}

Particle::Particle(Vect3 pos, Vect3 vel, Vect3 acc,
	float mass, float gravity, float damping) :
	m_pos(pos), m_vel(vel), m_acc(acc),
	m_inverseMass((mass > 0) ? (1.0f / mass) : (0.0f)),
	m_g(gravity),
	m_damp(damping)
{}

void Particle::AugmentNetForce(Vect3 toAdd) {
	m_netForces += toAdd;
}

Vect3 Particle::GetPosition() {
	return m_pos;
}

Vect3 Particle::GetAcceleration() {
	return m_acc;
}

Vect3 Particle::GetVelocity() {
	return m_vel;
}

bool Particle::IsFiniteMass() {
	return (0 != m_inverseMass);
}

float Particle::GetMass() {
	if (m_inverseMass == 0)
		return -1.0f;
	else
		return (1.0f / m_inverseMass);
}

float Particle::GetInverseMass() {
	return m_inverseMass;
}

float Particle::GetDrag() {
	return m_damp;
}

void Particle::SetMass(float mass) {
	assert(mass > 0.0f);
	m_inverseMass = 1.0f / mass;
}

void Particle::SetInverseMass(float inverseMass) {
	m_inverseMass = inverseMass;
}

void Particle::SetPosition(Vect3 newPos) {
	m_pos = newPos;
}

void Particle::SetVelocity(Vect3 newVel) {
	m_vel = newVel;
}

void Particle::SetAcceleration(Vect3 newAcc) {
	m_acc = newAcc;
	m_netForces = newAcc * (1.0f / m_inverseMass);
}

void Particle::SetGravity(float newG) {
	m_g = newG;
}

float Particle::GetGravity() {
	return m_g;
}

void Particle::SetDamping(float newDampingConstant) {
	assert(newDampingConstant >= 0.0f);
	assert(newDampingConstant <= 1.0f);
	m_damp = newDampingConstant;
}

void Particle::Integrate(float timeElapsed) {
	assert(timeElapsed > 0.0f);

	// Work out acceleration from the force...
	m_acc = (m_netForces * m_inverseMass);

	// Update linear velocity
	m_vel += (m_acc * timeElapsed);

	// Update linear position...
	m_pos += (m_vel * timeElapsed);

	// Impose drag *cringe*
	//  Optimization - don't use full pow() function, use
	//  a cheaper estimate (this number is ALWAYS very small)
	m_vel *= (pow(m_damp, timeElapsed));

	// Clear net forces for the next frame
	//  Object will continue undisturbed until the force
	//  register is updated.
	m_netForces = Vect3(0.0f, 0.0f, 0.0f);
}