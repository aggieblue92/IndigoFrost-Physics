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
float BoundingSphere::getGrowth(const BoundingSphere& av) {
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

	// Growth is how much bigger dist / 2 is than the current radius.
	return (dist * 0.5f).Magnitude() - m_radius;
}

// UNTESTED
void BoundingSphere::getNewBoundingSphere(BoundingSphere& o_newSphere, const BoundingSphere& toAdd) {
	// Find distance between the two.
	Vect3 dist = toAdd.getLocation() - this->getLocation();
	Vect3 tail = dist - this->getRadius();

	// Next, find how much they are interpenetrating by.
	float penetration = toAdd.getRadius() + this->getRadius() - dist.Magnitude();
	if(penetration <= 0.f)
		penetration = 0.f;

	// Subtract penetration... TEST THIS LINE.
	dist -= penetration;

	// Now, factor in the radii - the average location is going to be the distance
	dist = dist + toAdd.getRadius() + this->getRadius();

	// Okay, so now our new location is dist / 2 + tail.
	o_newSphere.setLocation((dist * 0.5f) + tail);

	// And the new radius is the magnitude of dist / 2.
	o_newSphere.setRadius((dist * 0.5f).Magnitude());
}