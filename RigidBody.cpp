#include "RigidBody.h"
#include "Quaternion.h"
using namespace Frost;

static inline void _calculateTransformMatrix(Matrix4& transformMatrix, const Vect3& position, const Quaternion& orientation) {
	transformMatrix.data[0] = 1 - 2 * orientation.j * orientation.j - 2 * orientation.k * orientation.k;
	transformMatrix.data[1] = 2*orientation.i*orientation.j - 2*orientation.r*orientation.k;
	transformMatrix.data[2] = 2*orientation.i*orientation.k + 2*orientation.r*orientation.j;
	transformMatrix.data[3] = position.x;
	transformMatrix.data[4] = 2*orientation.i*orientation.j + 2*orientation.r*orientation.k;
	transformMatrix.data[5] = 1-2*orientation.i*orientation.i-2*orientation.k*orientation.k;
	transformMatrix.data[6] = 2*orientation.j*orientation.k - 2*orientation.r*orientation.i;
	transformMatrix.data[7] = position.y;
	transformMatrix.data[8] = 2*orientation.i*orientation.k - 2*orientation.r*orientation.j;
	transformMatrix.data[9] = 2*orientation.j*orientation.k + 2*orientation.r*orientation.i;
	transformMatrix.data[10] = 1-2*orientation.i*orientation.i- 2*orientation.j*orientation.j;
	transformMatrix.data[11] = position.z;
}

static inline void _transformInertiaTensor(Matrix3& iitWorld, const Quaternion& q, const Matrix3& iitBody, const Matrix4& rotmat) {
	float t4 = rotmat.data[0]*iitBody.data[0]+
		rotmat.data[1]*iitBody.data[3]+
		rotmat.data[2]*iitBody.data[6];
	float t9 = rotmat.data[0]*iitBody.data[1]+
		rotmat.data[1]*iitBody.data[4]+
		rotmat.data[2]*iitBody.data[7];
	float t14 = rotmat.data[0]*iitBody.data[2]+
		rotmat.data[1]*iitBody.data[5]+
		rotmat.data[2]*iitBody.data[8];
	float t28 = rotmat.data[4]*iitBody.data[0]+
		rotmat.data[5]*iitBody.data[3]+
		rotmat.data[6]*iitBody.data[6];
	float t33 = rotmat.data[4]*iitBody.data[1]+
		rotmat.data[5]*iitBody.data[4]+
		rotmat.data[6]*iitBody.data[7];
	float t38 = rotmat.data[4]*iitBody.data[2]+
		rotmat.data[5]*iitBody.data[5]+
		rotmat.data[6]*iitBody.data[8];
	float t52 = rotmat.data[8]*iitBody.data[0]+
		rotmat.data[9]*iitBody.data[3]+
		rotmat.data[10]*iitBody.data[6];
	float t57 = rotmat.data[8]*iitBody.data[1]+
		rotmat.data[9]*iitBody.data[4]+
		rotmat.data[10]*iitBody.data[7];
	float t62 = rotmat.data[8]*iitBody.data[2]+
		rotmat.data[9]*iitBody.data[5]+
		rotmat.data[10]*iitBody.data[8];

	iitWorld.data[0] = t4*rotmat.data[0]+
		t9*rotmat.data[1]+
		t14*rotmat.data[2];
	iitWorld.data[1] = t4*rotmat.data[4]+
		t9*rotmat.data[5]+
		t14*rotmat.data[6];
	iitWorld.data[2] = t4*rotmat.data[8]+
		t9*rotmat.data[9]+
		t14*rotmat.data[10];
	iitWorld.data[3] = t28*rotmat.data[0]+
		t33*rotmat.data[1]+
		t38*rotmat.data[2];
	iitWorld.data[4] = t28*rotmat.data[4]+
		t33*rotmat.data[5]+
		t38*rotmat.data[6];
	iitWorld.data[5] = t28*rotmat.data[8]+
		t33*rotmat.data[9]+
		t38*rotmat.data[10];
	iitWorld.data[6] = t52*rotmat.data[0]+
		t57*rotmat.data[1]+
		t62*rotmat.data[2];
	iitWorld.data[7] = t52*rotmat.data[4]+
		t57*rotmat.data[5]+
		t62*rotmat.data[6];
	iitWorld.data[8] = t52*rotmat.data[8]+
		t57*rotmat.data[9]+
		t62*rotmat.data[10];
}

RigidBody::RigidBody(Vect3 pos, Vect3 vel, Vect3 acc,
			Quaternion orientation, Vect3 rotation, Vect3 angularAcc,
			float mass, float gravity, float damping,
			float angularDamping, Matrix3 inverseInertiaTensor)
			: m_position(pos), m_velocity(vel), m_acceleration(acc),
			m_orientation(orientation), m_rotation(rotation),
			m_angularAcceleration(angularAcc),
			m_g(gravity), m_damp(damping),
			m_angularDamping(angularDamping),
			m_inverseInertiaTensor(inverseInertiaTensor)
{
	assert(mass > 0.f);
	m_inverseMass = 1.f / mass;

	this->calculateDerivedData();
}

void RigidBody::calculateDerivedData() {
	_calculateTransformMatrix(m_transformMatrix, m_position, m_orientation);
	_transformInertiaTensor(m_inverseInertiaTensorWorld,
		m_orientation, m_inverseInertiaTensor, m_transformMatrix);
}

void RigidBody::setInertiaTensor(const Matrix3& inverseInertiaTensor) {
	m_inverseInertiaTensorWorld = inverseInertiaTensor;
}

void RigidBody::addForce(const Vect3& force) {
	this->m_netForces += force;
}

void RigidBody::addForceAtBodyPoint(const Vect3& force_world, const Vect3& point_local) {
	// Convert to coordinates relative to center of mass...
	//worldToLocal
	Vect3 pt = localToWorld(point_local, m_transformMatrix);
	addForceAtPoint(force_world, pt);
}

// ??? I'm pretty sure, but really I'm shakey about all of this.
void RigidBody::addForceAtPoint(const Vect3& force, const Vect3& pt) {
	// Add a force to the thinger at this point.
	//  For now, just handle the torque. I'll figure out
	//  the linear motion later... TODO TODO
	Vect3 torque = Vect3::CrossProduct(pt, force);
	m_netTorque += torque;

	// VERRY VERY TEMPORARY adding of a force...
	m_netForces += force;
}

void RigidBody::Integrate(float timeElapsed) {
	// Calculate linear acceleration from force inputs
	m_lastFrameAcceleration = m_acceleration;
	m_lastFrameAcceleration += (m_netForces * m_inverseMass);

	// Calculate angular acceleration from torque inputs
	m_angularAcceleration = m_inverseInertiaTensorWorld * m_netTorque;

	// Adjust velocities...
	// Linear velocity from acceleration and impulse
	m_velocity += (m_lastFrameAcceleration * timeElapsed);

	// Angular velocity from both acceleration and impulse
	m_rotation += (m_angularAcceleration * timeElapsed);

	// Adjust positions...
	m_position += (m_velocity * timeElapsed);
	m_orientation.addScaledVector(m_rotation, timeElapsed);

	// Impose drag...
	m_velocity *= std::pow(m_damp, timeElapsed);
	m_rotation *= std::pow(m_angularDamping, timeElapsed);

	// Normalize the orientation quaternion, update matrices
	//  with new position and orientation
	calculateDerivedData();

	// Clear the accumulators...
	m_netForces = Vect3(0.f, 0.f, 0.f);
	m_netTorque = Vect3(0.f, 0.f, 0.f);
}

bool RigidBody::IsFiniteMass() {
	return (0 != m_inverseMass);
}

float RigidBody::getGravity() {
	return m_g;
}

float RigidBody::getDamping() {
	return m_damp;
}

float RigidBody::GetMass() {
	assert(m_inverseMass > 0.f);
	
	return 1.f / m_inverseMass;
}

float RigidBody::GetInverseMass() {
	return m_inverseMass;
}