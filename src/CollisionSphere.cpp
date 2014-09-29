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
, _radius(other.GetRadius())
{}

bool CollisionSphere::isTouching(ICollisionGeometry* other) const
{
	if (other->GetType() == FROST_COLLISION_GEOMETRY_TYPE_BOX)
	{
		return isTouchingB((CollisionBox*)other);
	}
	else if (other->GetType() == FROST_COLLISION_GEOMETRY_TYPE_SPHERE)
	{
		return isTouchingS((CollisionSphere*)other);
	}
	else
	{
		throw UnsupportedCollisionGeometryEvent(_type, other->GetType());
	}
}

void CollisionSphere::genContacts(ICollisionGeometry* other, std::vector<IContact*>& o_list) const
{
	if (other->GetType() == FROST_COLLISION_GEOMETRY_TYPE_BOX)
	{
		genContactsB((CollisionBox*)other, o_list);
	}
	else if (other->GetType() == FROST_COLLISION_GEOMETRY_TYPE_SPHERE)
	{
		genContactsS((CollisionSphere*)other, o_list);
	}
	else
	{
		throw UnsupportedCollisionGeometryEvent(_type, other->GetType());
	}
}

float CollisionSphere::GetRadius() const
{
	return _radius;
}

bool CollisionSphere::isTouchingB(CollisionBox* b) const
{
	// Get the sphere position in local coordinates of the box.
	Vect3 transformedSpherePosition = b->GetTransformMatrix().GetInverse() * this->GetPos();

	// If the sphere is further away than the magnitude of the furthest distance
	//  a corner can be, early out:
	if (transformedSpherePosition.SquareMagnitude() > b->getSize().SquareMagnitude())
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
	return ((closestBoxPoint - transformedSpherePosition).SquareMagnitude() <= this->GetRadius() * this->GetRadius());
}

// TODO: Test!
// REALLY TODO: TEST. There definitely is a bug in here.
void CollisionSphere::genContactsB(CollisionBox* b, std::vector<IContact*>& o) const
{
	// Get the sphere position in local coordinates of the box.
	Vect3 transformedSpherePosition = b->GetTransformMatrix().GetInverse() * this->GetPos();

	// If the sphere is further away than the magnitude of the furthest distance
	//  a corner can be, early out:
	if (transformedSpherePosition.SquareMagnitude() > b->getSize().SquareMagnitude())
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
	if ((closestBoxPoint - transformedSpherePosition).SquareMagnitude() < this->GetRadius() * this->GetRadius()
		&& ((closestBoxPoint - transformedSpherePosition).SquareMagnitude() > 0.f))
	{
		// Box contact data: Contact is under the surface of the sphere, pointing directly out.
		Vect3Normal d = (closestBoxPoint - transformedSpherePosition);
		Vect3 collisionPoint_l = transformedSpherePosition + d * _radius;
		Vect3 penetration_l = closestBoxPoint - collisionPoint_l;
		Vect3 collisionPoint_w = b->GetTransformMatrix() * collisionPoint_l;
		Vect3 penetration_w = b->GetTransformMatrix().TransformDirn(penetration_l);
		/*Vect3 collisionPoint_l = (Vect3Normal(closestBoxPoint - this->GetPos()) * this->GetRadius()) + this->GetPos();
		Vect3 collisionPoint_w = b->GetTransformMatrix() * (closestBoxPoint - transformedSpherePosition);
		Vect3 penetration_w = (Vect3Normal(collisionPoint_w - this->GetPos()) * this->GetRadius()) - collisionPoint_w;*/
		o.push_back(this->SummonDemons(
			collisionPoint_w,
			penetration_w,
			b->GetAttachedObjectPtr(), _attachedObject));

		// Sphere contact data: Exact opposite of the box contact.
		penetration_w *= -1.f;
		o.push_back(this->SummonDemons(
			collisionPoint_w,
			penetration_w,
			this->GetAttachedObjectPtr(), b->GetAttachedObjectPtr()));
	}
}

bool CollisionSphere::isTouchingS(CollisionSphere* s) const
{
	// The distance is the difference between the two objects...
	float squareDistance = (s->GetPos() - this->GetPos()).SquareMagnitude();

	// The spheres are touching if the magnitude of the distance is less
	//  than the sum of the radii
	float squareRadius = s->GetRadius() + this->GetRadius();
	squareRadius *= squareRadius;

	return squareDistance <= squareRadius;
}

void CollisionSphere::genContactsS(CollisionSphere* s, std::vector<IContact*>& o) const
{
	// Get distance between two objects, with this one at the origin...
	Vect3 distance = s->GetPos() - this->GetPos();

	// Spheres are touching if the magnitude of the distance is less than
	//  the sum of the radii
	// Early out if this is not the case.
	if (distance.SquareMagnitude() > ((this->GetRadius() + s->GetRadius()) * (this->GetRadius() + s->GetRadius())))
	{
		return;
	}
	else
	{
		// Generate contacts!
		// Contact is midpoint in world space of intersection,
		// normal is direction of intersection.
		Vect3Normal directionNormal = (s->GetPos() - this->GetPos());
		float contactMagnitude = this->GetRadius() + s->GetRadius() - distance.Magnitude();

		// From the other object, go to the edge of the sphere.
		//  Subtract that from the edge of this sphere.
		//  now multiply the whole thing by 1/2, and add the edge of this sphere.
		// That's the midpoint of the intersected space of the two spheres.
		Vect3 contactPoint = ((s->GetPos() - directionNormal * s->GetRadius())
			- (this->GetPos() + directionNormal * this->GetRadius()))
			* 0.5f
			+ (this->GetPos() + directionNormal * this->GetRadius());
		o.push_back(SummonDemons(contactPoint, directionNormal * contactMagnitude * -1.f, s->GetAttachedObjectPtr(), _attachedObject));
		o.push_back(SummonDemons(contactPoint, directionNormal * contactMagnitude, this->GetAttachedObjectPtr(), s->GetAttachedObjectPtr()));
	}
}

IContact* CollisionSphere::SummonDemons(const Vect3& pt, const Vect3& pen, IPhysicsObject* obj, IPhysicsObject* oobj) const
{
	// This function is here so that overriding this class is easier - all contact generation
	//  goes in here, though the information is generated outside of the function
	return new BasicContact(pt, pen, obj);
}