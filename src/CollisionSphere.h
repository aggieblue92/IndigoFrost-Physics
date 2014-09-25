/////////////////////////////////////////
// CollisionSphere: ICollisionGeometry object
//  that represents a sphere.
// Uses the BasicContact for collisions
/////////////////////////////////////////

#ifndef FROST_COLLISION_SPHERE_H
#define FROST_COLLISION_SPHERE_H

#include "ICollisionGeometry.h"
#include "BasicContact.h"

namespace Frost
{
	class CollisionSphere : public ICollisionGeometry
	{
	public:
		/////////////////// CTORS //////////////////////

		// Construct a sphere with the given dimensions, position and orientation
		CollisionSphere(float radius, const FLOAT3& pos);

		// Copy ctor
		CollisionSphere(const CollisionSphere& other);

		//////////////////// OVERRIDES //////////////////

		// Returns whether or not the box is in contact with the other collision geometry
		virtual bool isTouching(ICollisionGeometry* other) const;

		// Appends a list of contacts (if any) between the two geometry objects to o_list
		virtual void genContacts(ICollisionGeometry* other, std::vector<IContact*>& o_list) const;

		////////////////////// HELPERS ////////////////////
		float GetRadius() const;

	protected:
		float _radius;
		float _collisionResolutionConstant;

	protected:
		virtual bool isTouchingB(CollisionBox* b) const;
		virtual void genContactsB(CollisionBox* b, std::vector<IContact*>& o) const;

		virtual bool isTouchingS(CollisionSphere* s) const;
		virtual void genContactsS(CollisionSphere* s, std::vector<IContact*>& o) const;
	};
}

#endif