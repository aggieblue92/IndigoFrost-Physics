#ifndef INDIGO_FROST_PLAYER_MOTION_FORCE_H_
#define INDIGO_FROST_PLAYER_MOTION_FORCE_H_

/*****************************************************\

		PlayerMotionForce: A force which can be used
	for smooth player motion.

This is a force generator which provides a player with
  strictly linear force - it is, however, smooth speed
  application. I.e., the force is a force, not just a
  sudden POOF velocity like some crap games.

Members:
  m_direction: The unit direction of object motion.
    Update this member to change object motion direction.
  m_speed: The maximum velocity of the object due only
    to this force.
  m_timeToMaxSpeed: The time it takes the object (with
    no other acting forces) to reach maximum velocity
	from a complete stop.
  m_active: Is this force active? Upon updating the force,
    this is set to false, so every frame where the player
	motion force is used, the SetActive() function should
	be called.

NOTE: I was having some problems with this force generator
  earlier, but they seem to have magically disappeared.
  I've long since gotten rid of my dev notes, but essentially
  this force 1) negates drag on this force at max speed, and
  2) applies a force so that with no other acting forces,
  the object has an acceleration of (maxVelocity / timeToMaxSpeed)
\*****************************************************/

#include "ForceGenerator.h"

namespace Frost {
	class PlayerMotionForce : public RigidBodyForceGenerator {
	public:
		// Ctor for player motion force.
		//  --maxVelocity: Direction and magnitude of maximum player velocity
		//  --accTime: How long does it take object from a stop to reach max speed?
		PlayerMotionForce(Frost::Vect3 maxVelocity, float accTime);
		virtual void updateForce(Frost::RigidBody* rb, float duration);

		// Activate this force for the current frames
		void SetActive();

		// Set direction (unit direction) of motion
		void SetDirection(Frost::Vect3 newDir);

		// Set maximum velocity due to this force
		void SetSpeed(float newSpeed);

		// Set how long it takes object from a stop to reach max velocity
		void SetTimeToMaxVelocity(float newTime);

	private:
		Frost::Vect3 m_direction;
		float m_speed;
		float m_timeToMaxSpeed;
		bool m_active;
	};
}

#endif
