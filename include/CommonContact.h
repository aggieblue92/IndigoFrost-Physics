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
#ifndef FROST_COMMON_CONTACT_H
#define FROST_COMMON_CONTACT_H

/////////////////////////////////////////
// CommonContact: Implementation of IContact
//  that is used by CommonCollision* objects.
//  Includes friction, bounciness, etc.
/////////////////////////////////////////

#include "IContact.h"
#include "IPhysicsObject.h"

namespace Frost
{
	class CommonContact : public IContact
	{
	public:
		CommonContact(const Vect3& pt, const Vect3& penetration, float bounciness, float friction, IPhysicsObject* receivingObject, IPhysicsObject* otherObject);

		virtual bool resolve(float dt);

	protected:
		IPhysicsObject* _affectedObject;
		IPhysicsObject* _otherObject;
		float _bounciness;
		float _friction;
	};
}

#endif