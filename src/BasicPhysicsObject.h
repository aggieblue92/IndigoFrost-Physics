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

#ifndef FROST_PHYSICS_OBJECT_BASIC_H
#define FROST_PHYSICS_OBJECT_BASIC_H

/////////////////////////////////////////
// BasicPhysicsObject: An IPhysicsObject
//  implementation for basic things, like
//  crates, balls, etc.
/////////////////////////////////////////

#include "IPhysicsObject.h"

namespace Frost
{
	class BasicPhysicsObject : public IPhysicsObject
	{
	public:
		BasicPhysicsObject(float invMass, const Matrix& invInertiaTensor,
			float linearDrag, float angularDrag,
			const FLOAT3& initPos = { 0.f, 0.f, 0.f },
			const Quaternion& initOrientation = MathConstants::QUATERNION_UNIT,
			const FLOAT3& initLinearVelocity = { 0.f, 0.f, 0.f },
			const FLOAT3& initAngularVelocity = { 0.f, 0.f, 0.f });

		BasicPhysicsObject(const BasicPhysicsObject& toCopy);

		virtual void addForceAtOrigin(const Vect3& forceToAdd_w);
		virtual void addForceAtPoint(const Vect3& forceToAdd_w, const Vect3& bodyPoint_w);
		virtual void addTorqueAtOrigin(const Vect3& torqueToAdd_w);

		virtual void update(float timeElapsed);
		virtual void impulse(const Vect3& applicationPoint, const Vect3& distance);

		float getLinearDrag() const;
		float getAngularDrag() const;

		void setLinearDrag(float linearDrag);
		void setAngularDrag(float angularDrag);

	protected:
		float _linearDrag;
		float _angularDrag;
	};
}

#endif