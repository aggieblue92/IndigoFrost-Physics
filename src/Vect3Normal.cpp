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
#include "Vect3Normal.h"
using namespace Frost;

/////////////////// CTORS //////////////////////
Vect3Normal::Vect3Normal() : Vect3(1.f, 0.f, 0.f) {}
Vect3Normal::Vect3Normal(float x, float y, float z)
: Vect3(x, y, z)
{
	float mag = this->magnitude();
	if (0.f == mag) throw ZeroMagnitudeException();
	_x /= mag;
	_y /= mag;
	_z /= mag;
}

Vect3Normal::Vect3Normal(const FLOAT3& o)
: Vect3(o)
{
	float mag = this->magnitude();
	if (0.f == mag) throw ZeroMagnitudeException();
	_x /= mag;
	_y /= mag;
	_z /= mag;
}

Vect3Normal::Vect3Normal(const Vect3& o)
: Vect3(o)
{
	float mag = this->magnitude();
	if (0.f == mag) throw ZeroMagnitudeException();
	_x /= mag;
	_y /= mag;
	_z /= mag;
}

Vect3Normal::Vect3Normal(const Vect3Normal& o)
: Vect3(o._x, o._y, o._z) {}

///////////// OPERATOR OVERLOADS /////////////
Vect3Normal& Vect3Normal::operator=(const FLOAT3& o)
{
	Vect3 other(o);

	// Check magnitude - throw exception on zero magnitude.
	float mag = other.magnitude();
	if (0.f == mag) throw ZeroMagnitudeException();

	this->_x = o._x / mag;
	this->_y = o._y / mag;
	this->_z = o._z / mag;
	return *this;
}

Vect3Normal& Vect3Normal::operator=(const Vect3Normal& o)
{
	this->_x = o._x;
	this->_y = o._y;
	this->_z = o._z;
	return *this;
}