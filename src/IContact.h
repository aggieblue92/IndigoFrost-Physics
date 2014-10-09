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
// IContact - Base interface for a contact
//  Holds some data about the collision
/////////////////////////////////////////

#ifndef FROST_CONTACT_INTERFACE_H
#define FROST_CONTACT_INTERFACE_H

#include "Movable.h"

namespace Frost
{
	class IContact
	{
	public:
		IContact(const Vect3& objPt, const Vect3& penetrationVector);

		// This method resolves or suggests a resolution between the objects in collision.
		// Returns: True if resolution succeeds, false if not.
		virtual bool resolve(float dt) = 0;

		Vect3 getCollisionPoint() const;
		Vect3Normal getContactNormal() const;
		float getContactMagnitude() const;

	protected:
		Vect3 _objCollisionPoint;

		Vect3Normal _contactNormal;
		float _contactMagnitude;
	};
}

#endif