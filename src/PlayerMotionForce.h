#ifndef FROST_FORCE_PLAYER_MOTION_FORCE_H
#define FROST_FORCE_PLAYER_MOTION_FORCE_H

#include "IForce.h"

/////////////////////////////////////////
// PlayerMotionForce: A force that allows
//  integration of smooth player motion
//  to be performed via physics engine.
/////////////////////////////////////////

namespace Frost
{
	class PlayerMotionForce : public IForce
	{
	public:
		PlayerMotionForce(const Vect3& maxVelocity, float accTime);
		virtual void ApplyForce(IPhysicsObject* affectedPhysicsObject, float duration);

		void setDirectionAndSpeed(const Vect3& dirn, float speed);
		void setMaxVelocity(float maxVelocity);
		void setDirection(const Vect3& dirn);
		void setTimeToMaxVelocity(float time);

		Vect3Normal getDirection() const;
		Vect3 getDirectionAndSpeed() const;
		float getVelocity() const;
		float getTimeToMaxVelodity() const;

	private:
		Vect3Normal _direction;
		float _speed;
		float _timeToMaxSpeed;
	};
}

#endif