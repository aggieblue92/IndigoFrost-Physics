/*
This source file is part of the Indigo Frost physics engine

The MIT License (MIT)

Copyright (c) 2014 Kamaron Peterson (aggieblue92)

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
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
		PlayerMotionForce(const PlayerMotionForce& o);
		virtual void applyForce(std::shared_ptr<IPhysicsObject> affectedPhysicsObject, float duration);

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