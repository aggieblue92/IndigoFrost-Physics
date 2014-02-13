#ifndef _RIGID_BODY_H_
#define _RIGID_BODY_H_

#include "PhysicsClass.h"

namespace Frost {
	class RigidBody {
	public:
		float m_inverseMass; // Easier than mass to use.
		Vect3 m_position;
		// Quaternion orientation;
		Vect3 m_velocity;
		Vect3 m_rotation;
		// Matrix4 transformMatrix; // Holds transform matrix for converting body space to world space, vice versa.

	};
}
#endif
