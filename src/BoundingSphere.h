#ifndef FROST_BVH_BOUNDING_SPHERE_H
#define FROST_BVH_BOUNDING_SPHERE_H

/////////////////////////////////////////
// BoundingSphere: Class used by the BVH
//  management system to perform coarse
//  collision detection.
/////////////////////////////////////////

#include "FrostMath.h"
#include "Collidable.h"

namespace Frost
{
	class BoundingSphere
	{
	public:
		BoundingSphere();
		BoundingSphere(const Vect3& pos, float radius);
		BoundingSphere(const BoundingSphere& o);
		BoundingSphere(const Collidable& c);

		void setPosition(const Vect3& newPos);
		void setRadius(float newRadius);

		Vect3 getPos() const;
		float getRadius() const;

		virtual bool isColliding(const Vect3& pt) const;
		virtual bool isColliding(const BoundingSphere& other) const;
		virtual float getVolume() const;

		// Return the new volume, should a new object be added to the bounding volume.
		virtual float getNewRadius(const BoundingSphere& volToAdd) const;
		static BoundingSphere getNewBoundingSphere(const BoundingSphere& sphere_1, const BoundingSphere& sphere_2);

	private:
		Vect3 _pos;
		float _radius;
	};
}

#endif
