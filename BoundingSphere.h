#ifndef INDIGO_FROST_BOUNDING_SPHERE_H
#define INDIGO_FROST_BOUNDING_SPHERE_H

/*****************************************************\

		Bounding Sphere - extension of BoundingVolume

	Simulates a sphere that has a point and radius.
Easy for calculating collisions, but usually not
very accurate except for sphere-like objects. Not like
canoes or people or buildings or anything really but
billiard balls, beach balls, pretty much just balls.

isColliding - easy. Find distance between center
  and point. Is it smaller than radius? Return that.
getVolume - Also easy. V=(4/3)PI*r^(3)

Upon implementation of other bounding primitives, you'll
  want to add isColliding methods between them too.

\*****************************************************/

#include "Vect3.h"
#include "RigidBody.h"

namespace Frost {
	class BoundingSphere {
	public:
		BoundingSphere();
		BoundingSphere(const Vect3& location, const float radius);

		void setLocation(const Vect3& newLocation);
		void setRadius(const float newRadius);

		Vect3 getLocation() const;
		float getRadius() const;

		virtual bool isColliding(Vect3 pt);
		virtual bool isColliding(const BoundingSphere& other);
		virtual float getVolume();

		// Return the new volume should a new object be added.
		virtual float getNewRadius(const BoundingSphere& addedVolume);
		static BoundingSphere getNewBoundingSphere(const BoundingSphere& sphere_1, const BoundingSphere& sphere_2);

		// Generates smallest completely enclosing bounding volume from rigid body.
		void fillFromRigidBody(const RigidBody& rb);

	private:
		// Location in world space of sphere.
		Vect3 m_location;
		// Radius of sphere.
		float m_radius;
	};
}

#endif
