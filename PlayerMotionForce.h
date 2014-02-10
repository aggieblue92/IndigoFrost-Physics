#ifndef _PLAYER_MOTION_FORCE_H_
#define _PLAYER_MOTION_FORCE_H_

#include "ForceGenerator.h"

namespace Frost {
	class PlayerMotionForce : public ParticleForceGenerator {
	public:
		PlayerMotionForce(Frost::Vect3 maxVelocity, float accTime);
		virtual void updateForce(Frost::Particle* p, float duration);

		void SetActive();
		void SetDirection(Frost::Vect3 newDir);
		void SetSpeed(float newSpeed);
		void SetTimeToMaxVelocity(float newTime);

	private:
		Frost::Vect3 m_direction;
		float m_speed;
		float m_timeToMaxSpeed;
		bool m_active;
	};
}

#endif
