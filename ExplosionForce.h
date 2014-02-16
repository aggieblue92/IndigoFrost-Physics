#pragma once // REMOVE THIS

#ifndef INDIGO_FROST_EXPLOSION_FORCE_H
#define INDIGO_FROST_EXPLOSION_FORCE_H

#include "ForceGenerator.h"

// Explosion force generator
//  Members: m_duration: Duration of explosion
//           m_explosionPower: Raw power (in Newtons / second) of explosion from source
//           m_timeRemaining: Time remaining in explosive force (in seconds)
//           m_sourcePos: Position of explosion (where it starts from in world coordinates)

// When activated, the explosion has (duration) time, and while it is still active,
//  it produces a force of m_explosionPower * timeElapsed / distanceFromParticle^2
// When inactive, it produces no force.

// Note: Implosions can also be simulated with a negative magnitude of power.

// TODO: Decide if you want this to only affect linear motion.
//  Really, that's not right.

namespace Frost {
	class ExplosionForce : public RigidBodyForceGenerator {
	public:
		ExplosionForce();
		ExplosionForce(float duration, float force_per_second, Vect3 sourcePos);
		virtual void updateForce(RigidBody* rb, float duration);

		// Getter/Setter functions
		void setDuration(float duration);
		void setForcePerSecond(float force_per_second);
		void setSourcePosition(Vect3 source);
		void Activate();

		float getDuration();
		float getForcePerSecond();
		float getTimeRemaining();
		Vect3 getSourcePosition();
		bool isActive();

	private:
		float m_duration;
		float m_explosionPower;
		float m_timeRemaining;
		Vect3 m_sourcePos;
	};
}
#endif
