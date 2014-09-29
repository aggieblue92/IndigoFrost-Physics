#ifndef FROST_COLLISION_BOX_COMMON_H
#define FROST_COLLISION_BOX_COMMON_H

/////////////////////////////////////////
// CommonCollisionBox: The most common
//  imagined usage of a collision box.
// Friction, bounciness, rolling, etc.
/////////////////////////////////////////

#include "CollisionBox.h"

namespace Frost
{
	class CommonCollisionBox : public CollisionBox
	{
	public:
		CommonCollisionBox(const FLOAT3& halfSize, const FLOAT3& pos, const Quaternion& orientation,
			float bounciness, float frictionCoefficient, IPhysicsObject* attachedObject);
		CommonCollisionBox(const CommonCollisionBox& o);

	protected:
		float _frictionCoefficient;
		float _bouncinessCoefficient;

	private:
		virtual IContact* SummonDemons(const Vect3& pt, const Vect3& pen, IPhysicsObject* thisObj, IPhysicsObject* other_obj) const;
	};
}

#endif