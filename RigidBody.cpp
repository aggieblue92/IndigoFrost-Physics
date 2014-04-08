#include "RigidBody.h"
#include "Quaternion.h"
using namespace Frost;

// Create and output a transformation matrix from a given orientation quaternion and translation
static inline void _calculateTransformMatrix(Matrix4& transformMatrix, const Vect3& position, const Quaternion& orientation) {
    transformMatrix.data[0] = 1-2*orientation.j*orientation.j-
        2*orientation.k*orientation.k;
    transformMatrix.data[1] = 2*orientation.i*orientation.j -
        2*orientation.r*orientation.k;
    transformMatrix.data[2] = 2*orientation.i*orientation.k +
        2*orientation.r*orientation.j;
    transformMatrix.data[3] = position.x;

    transformMatrix.data[4] = 2*orientation.i*orientation.j +
        2*orientation.r*orientation.k;
    transformMatrix.data[5] = 1-2*orientation.i*orientation.i-
        2*orientation.k*orientation.k;
    transformMatrix.data[6] = 2*orientation.j*orientation.k -
        2*orientation.r*orientation.i;
    transformMatrix.data[7] = position.y;

    transformMatrix.data[8] = 2*orientation.i*orientation.k -
        2*orientation.r*orientation.j;
    transformMatrix.data[9] = 2*orientation.j*orientation.k +
        2*orientation.r*orientation.i;
    transformMatrix.data[10] = 1-2*orientation.i*orientation.i-
        2*orientation.j*orientation.j;
    transformMatrix.data[11] = position.z;
}

// Transform inertia tensor, derive data for local and world inverse inertia tensor
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
			m_g(gravity), m_damp(damping),
			m_angularDamping(angularDamping),
			m_inverseInertiaTensor(inverseInertiaTensor),
			m_collisionGeometry(0)
{
	assert(mass > 0.f);
	m_inverseMass = 1.f / mass;

	this->calculateDerivedData();
}

RigidBody::RigidBody(Vect3 pos, Quaternion orientation, float mass,
	Matrix3 inverseInertiaTensor, float gravity,
	float damping, float angularDamping)
	: m_position(pos), m_velocity(Vect3(0.f, 0.f, 0.f)), m_acceleration(Vect3(0.f, 0.f, 0.f)),
	m_orientation(orientation), m_rotation(Vect3(0.f, 0.f, 0.f)),
	m_g(gravity), m_damp(damping), m_angularDamping(angularDamping),
	m_collisionGeometry(0), m_inverseInertiaTensor(inverseInertiaTensor)
{
	assert(mass > 0.f);
	m_inverseMass = 1.f / mass;
	this->calculateDerivedData();
}

RigidBody::RigidBody(const RigidBody& other) {
	this->m_inverseMass = other.m_inverseMass;
	this->m_inverseInertiaTensor = other.m_inverseInertiaTensor;
	this->m_inverseInertiaTensorWorld = other.m_inverseInertiaTensorWorld;
	this->m_position = other.m_position;
	this->m_velocity = other.m_velocity;
	this->m_acceleration = other.m_acceleration;
	this->m_orientation = other.m_orientation;
	this->m_rotation = other.m_rotation;
	this->m_transformMatrix = other.m_transformMatrix;
	this->m_collisionGeometry = other.m_collisionGeometry;
	this->m_g = other.m_g;
	this->m_damp = other.m_damp;
	this->m_angularDamping = other.m_angularDamping;
	this->m_lastFrameAcceleration = other.m_lastFrameAcceleration;
	this->m_netForces = other.m_netForces;
	this->m_netTorque = other.m_netTorque;
}

void RigidBody::calculateDerivedData() {
	// Normalize orientation quaternion,
	//  update transformation matrix.
	// This has to be done to make sure that a proper number of
	//  degrees of freedom is maintained by all member variables.
	m_orientation.normalize();
	_calculateTransformMatrix(m_transformMatrix, m_position, m_orientation);
	_transformInertiaTensor(m_inverseInertiaTensorWorld,
		m_orientation, m_inverseInertiaTensor, m_transformMatrix);
}

void RigidBody::addCollisionGeometry(Geometry* in) {
	m_collisionGeometry.push_back(in);
}
void RigidBody::clearCollisionGeometry() {
	for (int i = 0; i < m_collisionGeometry.size(); i++) {
		delete m_collisionGeometry[i];
		m_collisionGeometry[i] = 0;
	}
	m_collisionGeometry.resize(0);
}

Geometry* RigidBody::getCollisionObject(int index) const {
	if (m_collisionGeometry.size() > index)
		return m_collisionGeometry[index];
	else
		return 0;
}

int RigidBody::getNumCollisionObjects() const {
	return m_collisionGeometry.size();
}

void RigidBody::setInertiaTensor(const Matrix3& inverseInertiaTensor) {
	m_inverseInertiaTensorWorld = inverseInertiaTensor;
}

void RigidBody::addForce(const Vect3& force) {
	// Add force to center of mass - no rotation required.
	this->m_netForces += force;
}

void RigidBody::addForceAtBodyPoint(const Vect3& force_world, const Vect3& point_local) {
	// Convert to coordinates relative to center of mass...
	Vect3 pt = getPointInWorldSpace(point_local);

	// Add force at the given point.
	addForceAtPoint(force_world, pt);
}

// ??? I'm pretty sure, but really I'm shakey about all of this.
void RigidBody::addForceAtPoint(const Vect3& force_world, const Vect3& pt_world) {
	// Convert to coordinates relative to center of mass
	Vect3 pt = pt_world;
	pt -= m_position;

	// Apply force and torque as per equations.
	m_netTorque += Vect3::CrossProduct(pt, force_world);

	m_netForces += force_world;
}

void RigidBody::Integrate(float timeElapsed) {
	// Calculate linear acceleration from force inputs
	m_lastFrameAcceleration = m_acceleration;
	m_lastFrameAcceleration += (m_netForces * m_inverseMass);

	// Calculate angular acceleration from torque inputs
	Vect3 angularAcceleration =
		m_inverseInertiaTensorWorld.transform(m_netTorque);

	// Adjust velocities...
	// Linear velocity from acceleration and impulse
	m_velocity += (m_lastFrameAcceleration * timeElapsed);

	// Angular velocity from both acceleration and impulse
	m_rotation += (angularAcceleration * timeElapsed);

	// Impose drag...
	m_velocity *= std::pow(m_damp, timeElapsed);
	m_rotation *= std::pow(m_angularDamping, timeElapsed);

	// Adjust positions...
	m_position += (m_velocity * timeElapsed);
	m_orientation.addScaledVector(m_rotation, timeElapsed);

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

Vect3 RigidBody::getPointInLocalSpace(const Vect3& pt_w) const {
	return m_transformMatrix.transformInverse(pt_w);
}

Vect3 RigidBody::getPointInWorldSpace(const Vect3& pt_l) const {
	return m_transformMatrix.transform(pt_l);
}

Vect3 RigidBody::getDirectionInLocalSpace(const Vect3& d_w) const {
	return m_transformMatrix.transformInverseDirection(d_w);
}

Vect3 RigidBody::getDirectionInWorldSpace(const Vect3& d_l) const {
	return m_transformMatrix.transformDirection(d_l);
}

Vect3 RigidBody::getPosition() const {
	return m_position;
}

Vect3 RigidBody::getRotation() const {
	return m_rotation;
}

Vect3 RigidBody::getVelocity() const {
	return m_velocity;
}

Matrix3 RigidBody::getInverseInertiaTensor() const {
	return m_inverseInertiaTensor;
}

Matrix3 RigidBody::getInverseInertiaTensorWorld() const {
	return m_inverseInertiaTensorWorld;
}

Quaternion RigidBody::getOrientation() const {
	return m_orientation;
}

void RigidBody::setPosition(const Vect3& newPos) {
	m_position = newPos;
}

void RigidBody::setOrientation(const Quaternion& newOr) {
	m_orientation = newOr;
}

void RigidBody::setVelocity(const Vect3& velocity) {
	m_velocity = velocity;
}

void RigidBody::setVelocity(const float x, const float y, const float z) {
	m_velocity.x = x;
	m_velocity.y = y;
	m_velocity.z = z;
}