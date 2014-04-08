#ifndef _RIGID_BODY_H_
#define _RIGID_BODY_H_

/*****************************************************\

		RigidBody: Contains information for a
	rigid body in the physics engine.

Members:
	m_inverseMass: Inverse of mass of object represented
	m_inverseInertiaTensor: Inverse of inertia tensor
	    of object represented.
	m_inverseInertiaTensorWorld: Inverse of inertia
	    tensor of object represented in world space.
	m_position: Position of object in 3D space
	m_velocity: Velocity of object in 3D space
	m_orientation: Quaternion representing orientation
	    in 3D space. Update this when updating rotation
	m_rotation: Angular velocity of object
	m_transformMatrix: 4x4 transform matrix for object
	m_g: Acceleration due to gravity for object
	m_damp: Linear dampening (i.e., drag)
	m_lastFrameAcceleration: acceleration realized in
	    the last frame.
	m_netForces: The net force realized to the object
	    during the current frame
	m_netTorque: The net torque realized to the object
	    during the current frame
	m_collisionGeometry: List of collision geometry

Note on inverse mass and inverse inertia tensor:
  In physics, for force, the formula F=ma is used.
    However, I already know what F is from the force
	generators in the engine. So, it's easier for the
	computer to store (1/m), so that the equation
	a = (1/m)F can be used. Also, it makes more sense
	for what a zero means - in this engine, it means
	that the object has infinite mass (like the earth
	or something like that)
  The inverse inertia tensor is likewise used for the
    same reason - a=T^{-1} * Torque.

\*****************************************************/

#include <vector>
#include "Vect3.h"
#include "Quaternion.h"
#include "Matrix4.h"
#include "Matrix3.h"
#include "CollisionGeometry.h"

namespace Frost {

	struct Contact;
	class Geometry;
	class Box;
	class Sphere;

	class RigidBody {
	public:
		// TODO: Simplify this, add multiple ctors
		RigidBody(Vect3 pos, Vect3 vel, Vect3 acc,
			Quaternion orientation, Vect3 rotation, Vect3 angularAcc,
			float mass, float gravity, float damping,
			float angularDamping, Matrix3 inverseInertiaTensor);
		RigidBody(Vect3 pos, Quaternion orientation, float mass,
			Matrix3 inverseInertiaTensor, float gravity = 9.81f,
			float damping = 0.8f, float angularDamping = 0.8f);
		RigidBody(const RigidBody& other);

		// TODO: Make all these data members private
		float m_inverseMass;
		Matrix3 m_inverseInertiaTensorWorld;
		Matrix3 m_inverseInertiaTensor;
		Vect3 m_position;
		Vect3 m_velocity;
		Vect3 m_acceleration;
		Quaternion m_orientation;
		Vect3 m_rotation;
		Matrix4 m_transformMatrix; // Holds transform matrix for converting body space to world space, vice versa.
		std::vector<Geometry*> m_collisionGeometry; // Holds list of collision geometry. POSSIBLE OPTIMIZATION: Also hold a low-qual set, quicker resolution?

		// Calculate internal data from state data. This is called
		//  after the body's state is altered directly (during integration)
		void calculateDerivedData();

		// Add a piece of collision geometry to our list.
		void addCollisionGeometry(Geometry*);
		void clearCollisionGeometry();
		Geometry* getCollisionObject(int index) const;
		// Get number of collision objects stored.
		int getNumCollisionObjects() const;

		// Set the inertia tensor of the object
		void setInertiaTensor(const Matrix3& inverseInertiaTensor);

		// Adds given force to center of mass of the rigid body.
		void addForce(const Vect3& force);

		/* Adds given force at the given point
		--force_world The force to apply in WORLD coords
		--point_local The point of application, in LOCAL coords
		*/
		void addForceAtBodyPoint(const Vect3& force_world, const Vect3& point_local);

		// Adds the given force at the given point... Yep.
		void addForceAtPoint(const Vect3& force_world, const Vect3& pt_world);

		// Returns true if the object has finite mass
		bool IsFiniteMass();

		// Returns gravity constant of object
		float getGravity();

		// Returns damping constant of object
		float getDamping();

		// Returns mass of object
		float GetMass();

		// Returns (1/mass) of object
		float GetInverseMass();

		// Set velocity of the object
		void setVelocity(const Vect3& vel);

		// Set velocity of the object
		void setVelocity(const float x, const float y, const float z);

		// Transform world point to object local point
		Vect3 getPointInLocalSpace(const Vect3& pt_w) const;

		// Transform local point to world point
		Vect3 getPointInWorldSpace(const Vect3& pt_l) const;

		// Transform world direction to local direction
		Vect3 getDirectionInLocalSpace(const Vect3& d_w) const;

		// Transform local direction to world direction
		Vect3 getDirectionInWorldSpace(const Vect3& d_l) const;

		// Get position of this thing.
		Vect3 getPosition() const;
		Vect3 getRotation() const;
		Vect3 getVelocity() const;
		Matrix3 getInverseInertiaTensor() const;
		Matrix3 getInverseInertiaTensorWorld() const;
		Quaternion getOrientation() const;

		void setPosition(const Vect3& newPos);
		void setOrientation(const Quaternion& newOrientation);

		// Update position, velocity, rotation, orientation based on
		//  torques and forces added in the current frame
		// --timeElapsed: time since last update (usually 1 frame)
		void Integrate(float timeElapsed);

	protected:
		float m_g, m_damp;
		float m_angularDamping;

		Vect3 m_lastFrameAcceleration;

		Vect3 m_netForces;
		Vect3 m_netTorque;

	private:
		RigidBody() {}
	};

	struct Contact {
		Vect3 contactPoint_wp;
		Vect3 contactNormal_wd;
		float closingVelocity_wd;
		float magnitude;
		RigidBody* rb[2];
		float restitution;

		Contact(RigidBody* rbSource, RigidBody* rbOther, Vect3 contactPoint, Vect3 contactNormal, float Magnitude, float Restitution)
			: contactPoint_wp(contactPoint), contactNormal_wd(contactNormal), magnitude(Magnitude), restitution(Restitution)
		{
			rb[0] = rbSource;
			rb[1] = rbOther;
			// Closing velocity in direction of contact normal is projection of source velocity onto it minus
			//  other velocity onto it (normal will be facing wrong way for second term, hence - instead of + )
			closingVelocity_wd = (rbSource->getVelocity() * contactNormal) - (rbOther->getVelocity() * contactNormal);
		}

		// Add friction and such here.
	};
}

#endif
