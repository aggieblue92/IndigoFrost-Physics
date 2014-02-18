#ifndef INDIGO_FROST_BASIC_SPRING_FORCE_H
#define INDIGO_FROST_BASIC_SPRING_FORCE_H

#include "ForceGenerator.h"

/*****************************************************\

		BasicSpringForce: A force generator simulating
	a spring anchored to another object

Spring force generated follows this formula, basic to
  physics. I forget the name of the law, or the dude
  who invented it. I'll say Gauss did it, he did like
  everything. I know it wasn't Gauss. Anywho.
  F=-kx
  where k is the spring constant (in N/m),
  and x is the amount the spring is compressed/expanded
  from its resting position

NOTE: This force only applies force from one object to
  another - to really simulate a spring connecting two
  objects, you'll need to use 2 BasicSpringForces.

Members:
   m_other: Other object to which this spring is attached
   m_springConstant: Spring constant of the spring
   m_restLength: Resting length of the spring
   m_connectionPoint: Connection point to described
     object (in updateForces function) in local coords
   m_otherConnectionPoint: Connection point to m_other
     in the local coordinates of m_other
\*****************************************************/

namespace Frost {
	class BasicSpringForce : public RigidBodyForceGenerator {
	public:
		// Default ctor - connection points are in local
		//  coordinates, and localConnectionPt describes
		//  the connection to the object which will be
		//  provided in the updateForce method
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
		Vect3 m_connectionPoint;
		Vect3 m_otherConnectionPoint;
	};
}
#endif