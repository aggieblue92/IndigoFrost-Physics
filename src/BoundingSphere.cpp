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
#include "BoundingSphere.h"
#include "CollisionSphere.h"
#include "CollisionBox.h"
using namespace Frost;

BoundingSphere::BoundingSphere()
: _pos(MathConstants::VECTOR_ZERO)
, _radius(0.f)
{}

BoundingSphere::BoundingSphere(const Vect3& pos, float radius)
: _pos(pos)
, _radius(radius)
{}

BoundingSphere::BoundingSphere(const BoundingSphere& o)
: _pos(o._pos)
, _radius(o._radius)
{}

BoundingSphere::BoundingSphere(const Collidable& c)
{
	// Go through all geometry, find the furthest point from the origin.
	float furthestDistance = 0.f;

	// Find the origin point of the sphere... In the case of attachment to an
	//  object, it is the object's origin. If not, it is the median point of all
	//  objects radii.
	Vect3 objOrigin;
	if (c.getAttachedObject() == 0)
	{
		for (int i = 0; i < c.getNumObjects(); ++i)
		{
			objOrigin += c.getTransform(i).getPosition();
		}
		objOrigin /= (float)c.getNumObjects();
	}
	else
	{
		objOrigin = c.getAttachedObject()->getPos();
	}

	for (int i = 0; i < c.getNumObjects(); ++i)
	{
		// Luckily, we are spheres and don't have to worry about orientation.

		// We do, however, have to worry about the distance of the new thing from wherever.
		//  Get the extra distance, which is the distance from the rigid body origin to the
		//  collision geometry origin. Both are in world space.
		float extraDistance =
			(c.getCollisionObject(i)->getPos() - objOrigin).magnitude();

		switch (c.getCollisionObject(i)->getType())
		{
		case FROST_COLLISION_GEOMETRY_TYPE_SPHERE:
			if (furthestDistance < ((CollisionSphere*)c.getCollisionObject(i))->getRadius() + extraDistance)
			{
				furthestDistance = ((CollisionSphere*)c.getCollisionObject(i))->getRadius() + extraDistance;
			}
			break;
		case FROST_COLLISION_GEOMETRY_TYPE_BOX:
			if (furthestDistance < ((CollisionBox*)c.getCollisionObject(i))->getSize().magnitude() + extraDistance)
			{
				furthestDistance = ((CollisionBox*)c.getCollisionObject(i))->getSize().magnitude() + extraDistance;
			}
			break;
		default: 
			throw NotImplementedException();
		}
	}

	// We now have the furthest collision point from this sphere. That is our new radius.
	_pos = objOrigin;
	_radius = furthestDistance;
}

void BoundingSphere::setPosition(const Vect3& newPos)
{
	_pos = newPos;
}

void BoundingSphere::setRadius(float newRad)
{
	_radius = newRad;
}

Vect3 BoundingSphere::getPos() const
{
	return _pos;
}

float BoundingSphere::getRadius() const
{
	return _radius;
}

bool BoundingSphere::isColliding(const Vect3& pt) const
{
	return ((pt - _pos).squareMagnitude() < (_radius * _radius));
}

bool BoundingSphere::isColliding(const BoundingSphere& o) const
{
	// Distance between sphere locations less than sum of radii.
	float sqDist = (_radius + o._radius);
	sqDist *= sqDist;

	return (o._pos - _pos).squareMagnitude() < sqDist;
}

float BoundingSphere::getVolume() const
{
	return ((4.f / 3.f) * MathConstants::PI * _radius * _radius * _radius);
}

float BoundingSphere::getNewRadius(const BoundingSphere& toAdd) const{
	// Find distance between the two points...
	Vect3 dist = toAdd._pos - _pos;
	Vect3Normal n = dist;

	// Find interpenetration amount
	float penetration = toAdd._radius + _radius - dist.magnitude();
	if (penetration <= 0.f) penetration = 0.f;
	dist -= n * penetration;

	// Now, factor in the radii - the average location is going to be the distance.
	dist = dist + n * (toAdd._radius + _radius);

	// New radius is simply the magnitude of the midpoint
	return (dist * 0.5f).magnitude();
}

BoundingSphere BoundingSphere::getNewBoundingSphere(const BoundingSphere& b1, const BoundingSphere& b2)
{
	//// Find the distance between the two...
	//Vect3 dist = b2._pos - b1._pos;
	//Vect3Normal normal = dist;
	//Vect3 tail = b1._pos - (normal * b1._radius);

	//// Next, find interpenetration
	//float penetration = b2._radius + b1._radius - dist.Magnitude();
	//if (penetration <= 0.f) penetration = 0.f;
	//penetration /= 2.f;

	//// Subtract penetration, factor in the radii
	//dist = normal * (dist.Magnitude() + b1._radius + b2._radius - penetration);

	//// New location is dist / 2 + tail
	//return BoundingSphere((dist * 0.5f) + tail, (dist).Magnitude());

	Vect3 dist = b2.getPos() - b1.getPos();
	Vect3Normal normal = dist;
	Vect3 tail = b1.getPos() - (normal * b1.getRadius());

	Vect3 midpoint = (b2.getPos() + (normal * b2.getRadius()) + tail) / 2.f;
	float radius = (midpoint - tail).magnitude();

	return BoundingSphere(midpoint, radius);
}