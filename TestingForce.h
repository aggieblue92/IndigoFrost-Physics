#ifndef INDIGO_FROST_TESTING_FORCE_H
#define INDIGO_FROST_TESTING_FORCE_H

#include "ForceGenerator.h"

namespace Frost {

	class TestingForce : public RigidBodyForceGenerator {
	public:
		TestingForce(Vect3 force, Vect3 pt)
			: m_force_local(force),
			m_pt_local(pt)
		{}

		virtual void updateForce(RigidBody* rb, float duration) {
			// Pretty much just add the force to the body, that's it.
			Vect3 force_world = localToWorldDirn(m_force_local, rb->m_transformMatrix);
			Vect3 pt_world = localToWorld(m_pt_local, rb->m_transformMatrix);

			rb->addForceAtPoint(force_world, pt_world);
		}

	private:
		Vect3 m_force_local;
		Vect3 m_pt_local;
	};

}
#endif
