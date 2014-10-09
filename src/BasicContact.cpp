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

#include "BasicContact.h"
using namespace Frost;

BasicContact::BasicContact(const Vect3& objPt, const Vect3& penetration, IPhysicsObject* receivingObject)
: IContact(objPt, penetration)
, _affectedObject(receivingObject)
{}

bool BasicContact::resolve(float dt)
{
	// If there is no attached object, GTFO
	if (_affectedObject == 0) return true;

	// Otherwise, handle it using the super simple
	//  spring formula with a pretty rigid spring.
	// F = -kx

	const float K = 350.f; // 350 N/m

	_affectedObject->addForceAtPoint(
		this->_contactNormal * this->_contactMagnitude * K * -1.f,
		this->_objCollisionPoint);

	return true;
}