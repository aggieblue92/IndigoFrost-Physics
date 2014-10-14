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
