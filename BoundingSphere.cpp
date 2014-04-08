#include "BoundingSphere.h"
using namespace Frost;

const float PI = 3.141592653f;

BoundingSphere::BoundingSphere()
	: m_location(Vect3(0.f, 0.f, 0.f)), m_radius(0.f)
{}

BoundingSphere::BoundingSphere(const Vect3& location, const float radius)
	: m_location(location), m_radius(radius)
{}

void BoundingSphere::setLocation(const Vect3& newLocation) {
	m_location = newLocation;
}

void BoundingSphere::setRadius(const float newRadius) {
	if (newRadius > 0.f) {
		m_radius = newRadius;
	}
}

Vect3 BoundingSphere::getLocation() const {
	return m_location;
}

float BoundingSphere::getRadius() const {
	return m_radius;
}

bool BoundingSphere::isColliding(Vect3 pt) {
	// For sphere, this is easy - is the distance
	//  between the sphere location and the point
	//  less than the radius?
	return ((pt-m_location).Magnitude() < m_radius);
}

bool BoundingSphere::isColliding(const BoundingSphere& other) {
	// Distance between sphere locations less than sum
	//  of radii?
	return ((other.getLocation() - m_location).Magnitude() <
		(m_radius + other.m_radius));
}

float BoundingSphere::getVolume() {
	// Also easy for a sphere.
	return ((4.f / 3.f) * PI * m_radius * m_radius * m_radius);
}

// UNTESTED
float BoundingSphere::getNewRadius(const BoundingSphere& av) {
	// First off, find the distance between the two objects
	Vect3 dist = av.getLocation() - this->getLocation();

	// Next, find how much they are interpenetrating by.
	float penetration = av.getRadius() + this->getRadius() - dist.Magnitude();
	if(penetration <= 0.f)
		penetration = 0.f;

	// Subtract penetration... TEST THIS LINE.
	dist -= penetration;

	// Now, factor in the radii - the average location is going to be the distance
	dist = dist + av.getRadius() + this->getRadius();

	// New radius is simply the magnitude of the midpoint.
	return (dist * 0.5f).Magnitude();
}
//
//// UNTESTED
//void BoundingSphere::getNewBoundingSphere(BoundingSphere& o_newSphere, const BoundingSphere& toAdd) {
//	// Find distance between the two.
//	Vect3 dist = toAdd.getLocation() - this->getLocation();
//	Vect3 tail = dist - this->getRadius();
//
//	// Next, find how much they are interpenetrating by.
//	float penetration = toAdd.getRadius() + this->getRadius() - dist.Magnitude();
//	if(penetration <= 0.f)
//		penetration = 0.f;
//
//	// Subtract penetration... TEST THIS LINE.
//	dist -= penetration;
//
//	// Now, factor in the radii - the average location is going to be the distance
//	dist = dist + toAdd.getRadius() + this->getRadius();
//
//	// Okay, so now our new location is dist / 2 + tail.
//	o_newSphere.setLocation((dist * 0.5f) + tail);
//
//	// And the new radius is the magnitude of dist / 2.
//	o_newSphere.setRadius((dist * 0.5f).Magnitude());
//}

BoundingSphere BoundingSphere::getNewBoundingSphere(const BoundingSphere& sphere_1, const BoundingSphere& sphere_2) {
	// Find the distance between the two.
	Vect3 dist = sphere_2.getLocation() - sphere_1.getLocation();
	Vect3 norm = dist.GetNormal();
	Vect3 tail = sphere_1.getLocation() - (norm * sphere_1.getRadius());

	// Next, find if they are interpenetrating.
	float penetration = sphere_2.getRadius() + sphere_1.getRadius() - dist.Magnitude();
	if (penetration <= 0.f)
		penetration = 0.f;

	// Subtract penetration.
	//dist -= norm * penetration;
	// Factor in the radii - average location is going to be in the midpoint of the
	//  distance plus the radii.
	dist = norm * (dist.Magnitude() + sphere_1.getRadius() + sphere_2.getRadius());

	// New location is dist / 2 + tail.

	return BoundingSphere((dist * 0.5f) + tail, (dist * 0.5f).Magnitude());
}

void BoundingSphere::fillFromRigidBody(const RigidBody& rb) {
	// Go through all geometry, find furthest point.
	float furthestDistance = 0.f;

	for (int i = 0; i < rb.getNumCollisionObjects(); i++) {
		// Luckily for us, we are spheres and don't have to worry about orientation for hyper-rectangles.

		// We do, however, have to worry about distance of the new thing from wherever.
		//  Get the extra distance, which is the distance from the rigid body origin to the collision
		//  geometry origin. Both are in world space.
		float extraDistance = 0.f;
		extraDistance = (rb.getCollisionObject(i)->getPosition() - rb.getPosition()).Magnitude();

		// NOTE: Right now, this is an approximation. For long, skinny hyper-rectangles, this can actually
		//  be really inaccurate. It's assuming that the collision geometry and the furthest point on the
		//  collision geometry are on the same line. This is the case for spheres, but only seldom for
		//  cubes. This is only called upon creation of a rigid body, so REALLY UPDATE THIS.

		switch (rb.getCollisionObject(i)->getType()) {
		case GEOMETRY_TYPE::SPHERE:
			Sphere* s;
			rb.getCollisionObject(i)->fillSphere(s);
			if (furthestDistance < s->getRadius() + extraDistance) {
				furthestDistance = s->getRadius() + extraDistance;
			}
			break;
		case GEOMETRY_TYPE::BOX:
			Box* b;
			rb.getCollisionObject(i)->fillBox(b);
			if (furthestDistance < b->getHalfSize().Magnitude() + extraDistance) {
				furthestDistance = b->getHalfSize().Magnitude() + extraDistance;
			}
			break;
		default:
			break;
		}
	}

	// We now have the furthest collision point from this sphere. That is our new radius. Fin.
	this->m_location = rb.getPosition();
	this->m_radius = furthestDistance;
}