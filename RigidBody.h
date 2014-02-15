#ifndef _RIGID_BODY_H_
#define _RIGID_BODY_H_

#include "PhysicsClass.h"
#include "Quaternion.h"
#include "Matrix4.h"
#include "Matrix3.h"

namespace Frost {
	class RigidBody {
	public:
		float m_inverseMass; // Easier than mass to use.
		Matrix3 m_inverseInertiaTensorWorld; // Stays permanent - this is the inertia tensor in world coords
		Matrix3 m_inverseInertiaTensor; // Holds the inverse of the inertia tensor used for torque/acceleration gen
		Vect3 m_position;
		Quaternion m_orientation;
		Vect3 m_velocity;
		Vect3 m_rotation;
		Matrix4 m_transformMatrix; // Holds transform matrix for converting body space to world space, vice versa.

		// Calculate internal data from state data. This is called
		//  after the body's state is altered directly (during integration)
		void calculateDerivedData();
		void setInertiaTensor(const Matrix3& inverseInertiaTensor);
	};
}

// Stopped on page 229

#endif
