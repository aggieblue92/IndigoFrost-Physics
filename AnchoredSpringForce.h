#ifndef INDIGO_FROST_ANCHORED_SPRING_FORCE_H
#define INDIGO_FROST_ANCHORED_SPRING_FORCE_H

#include "ForceGenerator.h"


/*****************************************************\

		AnchoredSpring: A force generator simulating
	a spring anchored to some position in world space
	and an object

Spring force generated follows this formula, basic to
  physics. I forget the name of the law, or the dude
  who invented it. I'll say Gauss did it, he did like
  everything. I know it wasn't Gauss. Anywho.
  F=-kx
  where k is the spring constant (in N/m),
  and x is the amount the spring is compressed/expanded
  from its resting position

Members:
  m_anchor: Anchor position in world space (one end
     of the spring is connected here)
  m_connectionPoint: Connection point in LOCAL COORDINATES
     of the attachment to the object called in updateForce
  m_springConstant: Spring constant of the spring (N/m) or whatever
  m_restLength: Resting length of the spring (meters or whatever)

\*****************************************************/

namespace Frost {
	class AnchoredSpring : public RigidBodyForceGenerator {
	public:
		// Anchor is attachment point in world coordinates,
		//  connectionPoint_local in local coordinates,
		//  springConstant in (N/m) and restLength in (m)
		// Other units are allowable.
		AnchoredSpring(Vect3 anchor, Vect3 connectionPoint_local,
			float springConstant,
			float restLength);
		virtual void updateForce(RigidBody* rb, float duration);

	private:
		Vect3 m_anchor;
		Vect3 m_connectionPoint; // In local coordinates on the body
		float m_springConstant;
		float m_restLength;
	};
}
#endif
