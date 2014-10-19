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
		friend class CollisionBox;

	public:
		/////////////////// CTORS //////////////////////

		// Construct a sphere with the given dimensions, position and attached object
		CollisionSphere(float radius, const FLOAT3& pos, std::shared_ptr<IPhysicsObject> attachedObject);

		// Copy ctor
		CollisionSphere(const CollisionSphere& other);

		//////////////////// OVERRIDES //////////////////

		// Returns whether or not the box is in contact with the other collision geometry
		virtual bool isTouching(const ICollisionGeometry& other) const;

		// Appends a list of contacts (if any) between the two geometry objects to o_list
		virtual void genContacts(const ICollisionGeometry& other, std::vector<std::shared_ptr<IContact>>& o_list) const;

		////////////////////// HELPERS ////////////////////
		float getRadius() const;

	protected:
		float _radius;

	protected:
		virtual bool isTouchingB(const CollisionBox& b) const;
		virtual void genContactsB(const CollisionBox& b, std::vector<std::shared_ptr<IContact>>& o) const;

		virtual bool isTouchingS(const CollisionSphere& s) const;
		virtual void genContactsS(const CollisionSphere& s, std::vector<std::shared_ptr<IContact>>& o) const;

	private:
		virtual std::shared_ptr<IContact> summonDemons(const Vect3&, const Vect3&, std::shared_ptr<IPhysicsObject>, std::shared_ptr<IPhysicsObject>) const;
	};
}

#endif