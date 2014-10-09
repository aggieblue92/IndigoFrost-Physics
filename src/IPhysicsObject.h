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
		float getMass() const;
		void setInverseMass(float newInverseMass);
		Matrix getInverseInertiaTensor() const;
		void setInverseInertiaTensor(const Matrix& newVal);

		bool isMutable() const;

		// Update the object to simulate one second having passed in the world
		virtual void update(float timeElapsed) = 0;

		// Impulse: Move a point on an object a distance in a direction, via linear and angular
		//  motion. Think of it as a 'blink'
		virtual void impulse(const Vect3& applicationPoint, const Vect3& distanceToMove) = 0;

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