#ifndef _RIGID_BODY_H_
#define _RIGID_BODY_H_

#include "PhysicsClass.h"
#include "Quaternion.h"
#include "Matrix4.h"
#include "Matrix3.h"

namespace Frost {
	class RigidBody {
	public:
		// TODO: Simplify this, add multiple ctors
		RigidBody(Vect3 pos, Vect3 vel, Vect3 acc,
			Quaternion orientation, Vect3 rotation, Vect3 angularAcc,
			float mass, float gravity, float damping,
			float angularDamping, Matrix3 inverseInertiaTensor);


		float m_inverseMass; // Easier than mass to use.
		Matrix3 m_inverseInertiaTensorWorld; // Stays permanent - this is the inertia tensor in world coords
		Matrix3 m_inverseInertiaTensor; // Holds the inverse of the inertia tensor used for torque/acceleration gen
		Vect3 m_position;
		Vect3 m_velocity;
		Vect3 m_acceleration;
		Quaternion m_orientation;
		Vect3 m_rotation;
		Vect3 m_angularAcceleration;
		Matrix4 m_transformMatrix; // Holds transform matrix for converting body space to world space, vice versa.

		// Calculate internal data from state data. This is called
		//  after the body's state is altered directly (during integration)
		void calculateDerivedData();
		void setInertiaTensor(const Matrix3& inverseInertiaTensor);

		// Adds given force to center of mass of the rigid body.
		void addForce(const Vect3& force);

		/* Adds given force at the given point
		@param force The force to apply in WORLD coords
		@param point The point of application, in LOCAL coords
		*/
		void addForceAtBodyPoint(const Vect3& force_world, const Vect3& point_local);

		// Adds the given force at the given point... Yep.
		void addForceAtPoint(const Vect3& force_world, const Vect3& pt_world);

		// Helpers from Particle
		bool IsFiniteMass();
		float getGravity();
		float getDamping();
		float GetMass();
		float GetInverseMass();

		void Integrate(float timeElapsed);

	protected:
		float m_g, m_damp;
		float m_angularDamping;

		Vect3 m_lastFrameAcceleration;

		Vect3 m_netForces;
		Vect3 m_netTorque;
	};
}

// Stopped on page 229

#endif
