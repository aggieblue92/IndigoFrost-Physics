#ifndef FROSTDLL_API
#ifdef _WINDLL
#define FROSTDLL_API __declspec(dllexport)
#else
#define FROSTDLL_API __declspec(dllimport)
#endif
#endif

/////////////////////////////////////////
// TestPhysics: Basic physics object
//  used in testing.
/////////////////////////////////////////

#ifndef FROST_TEST_PHYSICS_OBJECT_H
#define FROST_TEST_PHYSICS_OBJECT_H

#include "IPhysicsObject.h"

namespace Frost
{
	class TestPhysicsObject : public IPhysicsObject
	{
	public:
		/////////////////// CTORS //////////////////////

		// Default ctor - origin, no orientation, no momentum.
		FROSTDLL_API TestPhysicsObject(float invMass, const Matrix& invInertiaTensor);

		// Copy ctor
		FROSTDLL_API TestPhysicsObject(const TestPhysicsObject& o);

		// From a position, orientation, velocities.
		FROSTDLL_API TestPhysicsObject(float invMass, const Matrix& invInertiaTensor, const FLOAT3& pos, const Quaternion& orientation, const FLOAT3& linearVelocity, const FLOAT3& angularVelocity);

		//////////////////// PHYSICS FUNCTIONS //////////////////
		///////////////////// OVERRIDES ////////////////////////

		// Adds a force to the object at the origin point (no angular acc. component)
		FROSTDLL_API virtual void AddForceAtOrigin(const Vect3& forceToAdd_w);

		// Adds a force to the object at a non-origin point
		FROSTDLL_API virtual void AddForceAtPoint(const Vect3& forceToAdd_w, const Vect3& bodyPoint);

		// Adds a torque to the object at the origin point.
		FROSTDLL_API virtual void AddTorqueAtOrigin(const Vect3& torqueToAdd_w);

		// Updates the object by clearing all accumulated forces and
		//  applying them to the momentum of the object.
		FROSTDLL_API virtual void Update(float timeElapsed);

	protected:
		float _invMass;
		Matrix _invInertiaTensor;
	};
}

#endif