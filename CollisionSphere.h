#ifndef FROSTDLL_API
#ifdef _WINDLL
#define FROSTDLL_API __declspec(dllexport)
#else
#define FROSTDLL_API __declspec(dllimport)
#endif
#endif

/////////////////////////////////////////
// CollisionSphere: ICollisionGeometry object
//  that represents a sphere.
/////////////////////////////////////////

#ifndef FROST_COLLISION_SPHERE_H
#define FROST_COLLISION_SPHERE_H

#include "ICollisionGeometry.h"

namespace Frost
{
	class CollisionSphere : public ICollisionGeometry
	{
	public:
		/////////////////// CTORS //////////////////////

		// Construct a sphere with the given radius
		FROSTDLL_API CollisionSphere(float radius);

		// Construct a sphere with the given dimensions, position and orientation
		FROSTDLL_API CollisionSphere(float radius, const FLOAT3& pos, const Quaternion& orientation);

		// Copy ctor
		FROSTDLL_API CollisionSphere(const CollisionSphere& other);

		//////////////////// OVERRIDES //////////////////

		// Returns whether or not the box is in contact with the other collision geometry
		FROSTDLL_API virtual bool isTouching(ICollisionGeometry* other) const;

		// Appends a list of contacts (if any) between the two geometry objects to o_list
		FROSTDLL_API virtual void genContacts(ICollisionGeometry* other, std::vector<IContact*>& o_list) const;

		////////////////////// HELPERS ////////////////////
		FROSTDLL_API float GetRadius() const;

	protected:
		float _radius;

	protected:
		FROSTDLL_API virtual bool isTouching(CollisionBox* b) const;
		FROSTDLL_API virtual void genContacts(CollisionBox* b, std::vector<IContact*>& o) const;

		FROSTDLL_API virtual bool isTouching(CollisionSphere* s) const;
		FROSTDLL_API virtual void genContacts(CollisionSphere* s, std::vector<IContact*>& o) const;
	};
}

#endif