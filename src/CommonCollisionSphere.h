#ifndef FROST_COLLISION_SPHERE_GENERAL_H
#define FROST_COLLISION_SPHERE_GENERAL_H

/////////////////////////////////////////
// CommonCollisionSphere: The most common
//  imagined usage of a collision sphere.
//  Friction, bounciness, rolling, etc.
/////////////////////////////////////////

#include "CollisionSphere.h"

namespace Frost
{
	class CommonCollisionSphere : public CollisionSphere
	{
	public:
		CommonCollisionSphere(float radius, const FLOAT3& pos, float bounciness, float frictionCoefficient, IPhysicsObject* attachedObject);
		CommonCollisionSphere(const CommonCollisionSphere& other);

	protected:
		float _frictionCoefficient;
		float _bouncinessCoefficient;

	private:
		virtual IContact* SummonDemons(const Vect3& pt, const Vect3& pen, IPhysicsObject* other_obj);
	};
}

#endif
