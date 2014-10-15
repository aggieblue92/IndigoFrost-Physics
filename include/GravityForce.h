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
#ifndef FROST_GRAVITY_FORCE_H
#define FROST_GRAVITY_FORCE_H

#include "IForce.h"

namespace Frost
{
	class GravityForce : public IForce
	{
	public:
		// Default ctor: gravity is 9.81 m/s in the negative y direction
		GravityForce(float objMass);

		// gravityDirection: Direction in which gravity points.
		// gravityMagnitude: Acceleration due to gravity (m/s^2)
		GravityForce(const Vect3Normal& gravityDirection, float gravityMagnitude);

		GravityForce(const GravityForce& o);

		// Apply the force of gravity to the specified object.
		virtual void applyForce(IPhysicsObject* affectedObject, float duration);

		// Get a pointer to a new gravity force object, the same as this one.
		virtual IForce* getNewForcePtr() const;

		// Sets the value of acceleration due to gravity
		void setMagnitude(float newMagnitude);

		// Changes the direction of gravity
		void setGravityDirection(const Vect3Normal& dir);

		// Returns the direction of gravity
		Vect3Normal getGravityDirection();

		// Gets the magnitude of the gravity force
		float getGravityMagnitude();

	protected:
		Vect3Normal _gravityDirection;
		float _gravityAcceleration;
	};
}

#endif