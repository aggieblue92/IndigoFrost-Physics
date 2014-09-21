/////////////////////////////////////////
// IPhysicsObject: Represents a Movable
//  that also has velocity and rotation,
//  that can absorb forces and process them.
/////////////////////////////////////////

#ifndef FROST_PHYSICS_OBJECT_H
#define FROST_PHYSICS_OBJECT_H

#include "Movable.h"

namespace Frost
{
	class IPhysicsObject : public Movable
	{
	public:
		/////////////////// CTORS //////////////////////

		// Default ctor - origin, no orientation, no motion
		IPhysicsObject();

		// Copy constructor
		IPhysicsObject(const IPhysicsObject& toCopy);

		// From a movable (no motion)
		IPhysicsObject(const Movable& inMovable);

		// From a position, orientation
		IPhysicsObject(const FLOAT3& pos, const Quaternion& orientation);

		// From a position, orientation, velocity, rotation
		IPhysicsObject(const FLOAT3& pos, const Quaternion& orientation, const FLOAT3& linearVelocity, const FLOAT3& rotationVelocity);

		//////////////////// PHYSICS FUNCTIONS //////////////////

		virtual void addForceAtOrigin(const Vect3& forceToAdd_w) = 0;
		virtual void addForceAtPoint(const Vect3& forceToAdd_w, const Vect3& bodyPoint) = 0;
		virtual void addTorqueAtOrigin(const Vect3& torqueToAdd_w) = 0;

		/////////////////////// ACCESSORS ///////////////////////
		

		// Update the object to simulate one second having passed in the world
		virtual void Update(float timeElapsed) = 0;

	protected:
		Vect3 _linearVelocity;
		Vect3 _angularVelocity;

		Vect3 _netForce;
		Vect3 _netTorque;
	};
}

#endif