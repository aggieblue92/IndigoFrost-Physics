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
		TestPhysicsObject(float invMass, const Matrix& invInertiaTensor);

		// Copy ctor
		TestPhysicsObject(const TestPhysicsObject& o);

		// From a position, orientation, velocities.
		TestPhysicsObject(float invMass, const Matrix& invInertiaTensor, const FLOAT3& pos, const Quaternion& orientation, const FLOAT3& linearVelocity, const FLOAT3& angularVelocity);

		//////////////////// PHYSICS FUNCTIONS //////////////////
		///////////////////// OVERRIDES ////////////////////////

		// Adds a force to the object at the origin point (no angular acc. component)
		virtual void addForceAtOrigin(const Vect3& forceToAdd_w);

		// Adds a force to the object at a non-origin point
		virtual void addForceAtPoint(const Vect3& forceToAdd_w, const Vect3& pt_world);

		// Adds a torque to the object at the origin point.
		virtual void addTorqueAtOrigin(const Vect3& torqueToAdd_w);

		// Updates the object by clearing all accumulated forces and
		//  applying them to the momentum of the object.
		virtual void Update(float timeElapsed);
	};
}

#endif