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