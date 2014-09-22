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

		// Full constructor
		IPhysicsObject(const FLOAT3& pos, const Quaternion& orientation,
			const Vect3& linearVelocity = MathConstants::VECTOR_ZERO,
			const Vect3& angularVelocity = MathConstants::VECTOR_ZERO,
			float invMass = 0.f,
			const Matrix& invInertiaTensor = MathConstants::MATRIX_ZERO);

		//////////////////// PHYSICS FUNCTIONS //////////////////

		virtual void addForceAtOrigin(const Vect3& forceToAdd_w) = 0;
		virtual void addForceAtPoint(const Vect3& forceToAdd_w, const Vect3& bodyPoint) = 0;
		virtual void addTorqueAtOrigin(const Vect3& torqueToAdd_w) = 0;

		/////////////////////// ACCESSORS ///////////////////////
		Vect3 getLinearVelocity() const;
		void setLinearVelocity(const FLOAT3& newLinearVelocity);
		Vect3 getAngularVelocity() const;
		void setAngularVelocity(const FLOAT3& newAngularVelocity);
		Vect3 getNetForce() const;
		Vect3 getNetTorque() const;

		float getInverseMass() const;
		void setInverseMass(float newInverseMass);
		Matrix getInverseInertiaTensor() const;
		void setInverseInertiaTensor(const Matrix& newVal);

		// Update the object to simulate one second having passed in the world
		virtual void Update(float timeElapsed) = 0;

	protected:
		Vect3 _linearVelocity;
		Vect3 _angularVelocity;

		Vect3 _netForce;
		Vect3 _netTorque;

		float _invMass;
		Matrix _invInertiaTensor;
	};
}

#endif