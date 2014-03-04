#include "BoundingSphere.h"
using namespace Frost;

const float PI = 3.14159f;

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

float BoundingSphere::getRadius() {
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