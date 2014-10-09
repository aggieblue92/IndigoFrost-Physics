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

#include "AnchoredSpringForce.h"
using namespace Frost;

AnchoredSpringForce::AnchoredSpringForce(const Vect3& w, const Vect3& l, float k, float r)
: _anchor_world(w)
, _connection_local(l)
, _springConstant(k)
, _restLength(r)
{}

AnchoredSpringForce::AnchoredSpringForce(const AnchoredSpringForce& o)
: _anchor_world(o._anchor_world)
, _connection_local(o._connection_local)
, _springConstant(o._springConstant)
, _restLength(o._restLength)
{}

void AnchoredSpringForce::applyForce(IPhysicsObject* affected, float dt)
{
	Vect3 conn_world = affected->getTransformMatrix() * _connection_local;
	Vect3 disp = _anchor_world - conn_world;
	float dx = disp.magnitude() - _restLength;
	affected->addForceAtPoint(Vect3Normal(disp) * dx * _springConstant, conn_world);
}

IForce* AnchoredSpringForce::getNewForcePtr() const
{
	return new AnchoredSpringForce(*this);
}