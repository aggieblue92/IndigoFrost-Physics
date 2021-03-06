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
// BasicContact - implementation of IContact
//  that does nothing but super basic stuff.
/////////////////////////////////////////

#ifndef FROST_BASIC_CONTACT_H
#define FROST_BASIC_CONTACT_H

#include "IContact.h"
#include "IPhysicsObject.h"

namespace Frost
{
	class BasicContact : public IContact
	{
	public:
		BasicContact(const Vect3& objPt, const Vect3& penetration, std::shared_ptr<IPhysicsObject> receivingObject);
		BasicContact(const Vect3& objPt, const Vect3Normal& penDirection, float penMagnitude, std::shared_ptr<IPhysicsObject> receivingObject);

		virtual bool resolve(float dt);

	protected:
		std::shared_ptr<IPhysicsObject> _affectedObject;
	};
}

#endif