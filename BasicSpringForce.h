#ifndef INDIGO_FROST_BASIC_SPRING_FORCE_H
#define INDIGO_FROST_BASIC_SPRING_FORCE_H

#include "ForceGenerator.h"

///////////////////////////////////////////////////////
// CLASS: BasicSpringForce
// DESC: A force generator that ties together two
//       particles in 3D space.
// REQS: Anchor: a particle to which the spring is
//          anchored (Particle)
//       Spring Constant in N/m (float)
//       Resting Length in m (float)
// NOTE: This only applies force (in updateForce)
//       to the particle supplied in updateForce,
//       so the anchor is unaffected.
///////////////////////////////////////////////////////

namespace Frost {
	class BasicSpringForce : public RigidBodyForceGenerator {
	public:
		BasicSpringForce(const Vect3& localConnectionPt,
			RigidBody* other,
			const Vect3& otherConnectionPt,
			float springConstant,
			float restLength);
		virtual void updateForce(RigidBody* rb, float duration);

	private:
		RigidBody* m_other;
		float m_springConstant;
		float m_restLength;

		// These are the connection points to the two objects
		//  IN LOCAL COORDINATES.
		Vect3 m_connectionPoint;
		Vect3 m_otherConnectionPoint;
	};
}
#endif