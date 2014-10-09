/*
This source file is part of the Indigo Frost physics engine

The MIT License (MIT)

Copyright (c) 2014 Kamaron Peterson (aggieblue92)

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "CollisionBox.h"
#include "CollisionSphere.h"
using namespace Frost;

#define CLAMP(x, min, max)	x < min ? min : (x > max ? max : x)

CollisionSphere::CollisionSphere(float radius, const FLOAT3& pos, IPhysicsObject* ipo)
: ICollisionGeometry(FROST_COLLISION_GEOMETRY_TYPE_SPHERE, pos, MathConstants::QUATERNION_UNIT, ipo)
, _radius(radius)
{}

CollisionSphere::CollisionSphere(const CollisionSphere& other)
: ICollisionGeometry(other)
, _radius(other.getRadius())
{}

bool CollisionSphere::isTouching(ICollisionGeometry* other) const
{
	if (other->getType() == FROST_COLLISION_GEOMETRY_TYPE_BOX)
	{
		return isTouchingB((CollisionBox*)other);
	}
	else if (other->getType() == FROST_COLLISION_GEOMETRY_TYPE_SPHERE)
	{
		return isTouchingS((CollisionSphere*)other);
	}
	else
	{
		throw UnsupportedCollisionGeometryEvent(_type, other->getType());
	}
}

void CollisionSphere::genContacts(ICollisionGeometry* other, std::vector<IContact*>& o_list) const
{
	if (other->getType() == FROST_COLLISION_GEOMETRY_TYPE_BOX)
	{
		genContactsB((CollisionBox*)other, o_list);
	}
	else if (other->getType() == FROST_COLLISION_GEOMETRY_TYPE_SPHERE)
	{
		genContactsS((CollisionSphere*)other, o_list);
	}
	else
	{
		throw UnsupportedCollisionGeometryEvent(_type, other->getType());
	}
}

float CollisionSphere::getRadius() const
{
	return _radius;
}

bool CollisionSphere::isTouchingB(CollisionBox* b) const
{
	// Get the sphere position in local coordinates of the box.
	Vect3 transformedSpherePosition = b->getTransformMatrix().getInverse() * this->getPos();

	// If the sphere is further away than the magnitude of the furthest distance
	//  a corner can be, early out:
	if (transformedSpherePosition.squareMagnitude() > b->getSize().squareMagnitude())
		return false;

	// Otherwise, find the closest point on the cube.
	//  For any axis, it will be the cube half length on that side if
	//  the sphere's position is further than the half length. If not, it'll
	//  just be the component on that side.
	Vect3 closestBoxPoint = transformedSpherePosition;
	closestBoxPoint._x = CLAMP(closestBoxPoint._x, -b->getSize()._x, b->getSize()._x);
	closestBoxPoint._y = CLAMP(closestBoxPoint._y, -b->getSize()._y, b->getSize()._y);
	closestBoxPoint._z = CLAMP(closestBoxPoint._z, -b->getSize()._z, b->getSize()._z);

	// Now we have the closest points on the box.
	//  If it's closer than the radius, we have a contact.
	return ((closestBoxPoint - transformedSpherePosition).squareMagnitude() <= this->getRadius() * this->getRadius());
}

// TODO: Test!
// REALLY TODO: TEST. There definitely is a bug in here.
void CollisionSphere::genContactsB(CollisionBox* b, std::vector<IContact*>& o) const
{
	// Get the sphere position in local coordinates of the box.
	Vect3 transformedSpherePosition = b->getTransformMatrix().getInverse() * this->getPos();

	// If the sphere is further away than the magnitude of the furthest distance
	//  a corner can be, early out:
	if (transformedSpherePosition.squareMagnitude() > b->getSize().squareMagnitude())
		return;

	// Otherwise, find the closest point on the cube.
	//  For any axis, it will be the cube half length on that side if
	//  the sphere's position is further than the half length. If not, it'll
	//  just be the component on that side.
	Vect3 closestBoxPoint = transformedSpherePosition;
	closestBoxPoint._x = CLAMP(closestBoxPoint._x, -b->getSize()._x, b->getSize()._x);
	closestBoxPoint._y = CLAMP(closestBoxPoint._y, -b->getSize()._y, b->getSize()._y);
	closestBoxPoint._z = CLAMP(closestBoxPoint._z, -b->getSize()._z, b->getSize()._z);

	// Now we have the closest point on the box.
	//  If it's closer than the radius, we have a contact
	if ((closestBoxPoint - transformedSpherePosition).squareMagnitude() < this->getRadius() * this->getRadius()
		&& ((closestBoxPoint - transformedSpherePosition).squareMagnitude() > 0.f))
	{
		// Box contact data: Contact is under the surface of the sphere, pointing directly out.
		Vect3Normal d = (closestBoxPoint - transformedSpherePosition);
		Vect3 collisionPoint_l = transformedSpherePosition + d * _radius;
		Vect3 penetration_l = closestBoxPoint - collisionPoint_l;
		Vect3 collisionPoint_w = b->getTransformMatrix() * collisionPoint_l;
		Vect3 penetration_w = b->getTransformMatrix().transformDirn(penetration_l);
		/*Vect3 collisionPoint_l = (Vect3Normal(closestBoxPoint - this->GetPos()) * this->GetRadius()) + this->GetPos();
		Vect3 collisionPoint_w = b->GetTransformMatrix() * (closestBoxPoint - transformedSpherePosition);
		Vect3 penetration_w = (Vect3Normal(collisionPoint_w - this->GetPos()) * this->GetRadius()) - collisionPoint_w;*/
		o.push_back(this->summonDemons(
			collisionPoint_w,
			penetration_w,
			b->getAttachedObjectPtr(), _attachedObject));

		// Sphere contact data: Exact opposite of the box contact.
		penetration_w *= -1.f;
		o.push_back(this->summonDemons(
			collisionPoint_w,
			penetration_w,
			this->getAttachedObjectPtr(), b->getAttachedObjectPtr()));
	}
}

bool CollisionSphere::isTouchingS(CollisionSphere* s) const
{
	// The distance is the difference between the two objects...
	float squareDistance = (s->getPos() - this->getPos()).squareMagnitude();

	// The spheres are touching if the magnitude of the distance is less
	//  than the sum of the radii
	float squareRadius = s->getRadius() + this->getRadius();
	squareRadius *= squareRadius;

	return squareDistance <= squareRadius;
}

void CollisionSphere::genContactsS(CollisionSphere* s, std::vector<IContact*>& o) const
{
	// Get distance between two objects, with this one at the origin...
	Vect3 distance = s->getPos() - this->getPos();

	// Spheres are touching if the magnitude of the distance is less than
	//  the sum of the radii
	// Early out if this is not the case.
	if (distance.squareMagnitude() > ((this->getRadius() + s->getRadius()) * (this->getRadius() + s->getRadius())))
	{
		return;
	}
	else
	{
		// Generate contacts!
		// Contact is midpoint in world space of intersection,
		// normal is direction of intersection.
		Vect3Normal directionNormal = (s->getPos() - this->getPos());
		float contactMagnitude = this->getRadius() + s->getRadius() - distance.magnitude();

		// From the other object, go to the edge of the sphere.
		//  Subtract that from the edge of this sphere.
		//  now multiply the whole thing by 1/2, and add the edge of this sphere.
		// That's the midpoint of the intersected space of the two spheres.
		Vect3 contactPoint = ((s->getPos() - directionNormal * s->getRadius())
			- (this->getPos() + directionNormal * this->getRadius()))
			* 0.5f
			+ (this->getPos() + directionNormal * this->getRadius());
		o.push_back(summonDemons(contactPoint, directionNormal * contactMagnitude * -1.f, s->getAttachedObjectPtr(), _attachedObject));
		o.push_back(summonDemons(contactPoint, directionNormal * contactMagnitude, this->getAttachedObjectPtr(), s->getAttachedObjectPtr()));
	}
}

IContact* CollisionSphere::summonDemons(const Vect3& pt, const Vect3& pen, IPhysicsObject* obj, IPhysicsObject* oobj) const
{
	// This function is here so that overriding this class is easier - all contact generation
	//  goes in here, though the information is generated outside of the function
	return new BasicContact(pt, pen, obj);
}