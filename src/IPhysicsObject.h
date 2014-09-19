#ifndef FROSTDLL_API
#ifdef _WINDLL
#define FROSTDLL_API __declspec(dllexport)
#else
#define FROSTDLL_API __declspec(dllimport)
#endif
#endif

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
		FROSTDLL_API IPhysicsObject();

		// Copy constructor
		FROSTDLL_API IPhysicsObject(const IPhysicsObject& toCopy);

		// From a movable (no motion)
		FROSTDLL_API IPhysicsObject(const Movable& inMovable);

		// From a position, orientation
		FROSTDLL_API IPhysicsObject(const FLOAT3& pos, const Quaternion& orientation);

		// From a position, orientation, velocity, rotation
		FROSTDLL_API IPhysicsObject(const FLOAT3& pos, const Quaternion& orientation, const FLOAT3& linearVelocity, const FLOAT3& rotationVelocity);

		//////////////////// PHYSICS FUNCTIONS //////////////////

		FROSTDLL_API virtual void AddForceAtOrigin(const Vect3& forceToAdd_w) = 0;
		FROSTDLL_API virtual void AddForceAtPoint(const Vect3& forceToAdd_w, const Vect3& bodyPoint) = 0;
		FROSTDLL_API virtual void AddTorqueAtOrigin(const Vect3& torqueToAdd_w) = 0;

		// Update the object to simulate one second having passed in the world
		FROSTDLL_API virtual void Update(float timeElapsed) = 0;

	protected:
		Vect3 _linearVelocity;
		Vect3 _angularVelocity;

		Vect3 _netForce;
		Vect3 _netTorque;
	};
}

#endif