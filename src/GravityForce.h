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
		virtual void ApplyForce(IPhysicsObject* affectedObject, float duration);

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