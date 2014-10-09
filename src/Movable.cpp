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

#include "Movable.h"
using namespace Frost;

Movable::Movable()
: _pos(0.f, 0.f, 0.f)
, _orientation(MathConstants::QUATERNION_UNIT)
, _isDirty(false)
, _transformMatrix(MathConstants::MATRIX_IDENTITY)
{}

Movable::Movable(const Movable& toCopy)
: _pos(toCopy._pos)
, _orientation(toCopy._orientation)
, _isDirty(toCopy._isDirty)
, _transformMatrix(toCopy._transformMatrix)
{}

Movable::Movable(const FLOAT3& pos)
: _pos(pos)
, _orientation(MathConstants::QUATERNION_UNIT)
, _isDirty(false)
, _transformMatrix(_orientation, _pos)
{}

Movable::Movable(const FLOAT3& pos, const Quaternion& orientation)
: _pos(pos)
, _orientation(orientation)
, _isDirty(false)
, _transformMatrix(orientation, pos)
{}

//////////////// GETTERS/SETTERS /////////////////

// Gets the world position of the object...
Vect3 Movable::getPos() const
{
	return _pos;
}

Quaternion Movable::getOrientation() const
{
	return _orientation;
}

void Movable::setPos(const Vect3& position)
{
	_pos = position;
	_isDirty = true;
}

void Movable::setOrientation(const Quaternion& orientation)
{
	_orientation = orientation;
	_isDirty = true;
}

void Movable::setTransformMatrix(const Matrix& transformMatrix)
{
	_transformMatrix = transformMatrix;
	_transformMatrix.getOrientationAndPosition(_pos, _orientation);
	_isDirty = false;
}

Matrix Movable::getTransformMatrix()
{
	if (_isDirty)
	{
		_transformMatrix = Matrix(_orientation, _pos);
		_isDirty = false;
	}

	return _transformMatrix;
}

Matrix Movable::getTransformMatrix() const
{
	if (_isDirty)
	{
		return Matrix(_orientation, _pos);
	}
	else
	{
		return _transformMatrix;
	}
}

//////////////// TRANSFORM OPS ////////////////////

void Movable::move(const FLOAT3& moveAmount_w)
{
	_pos += moveAmount_w;
	_isDirty = true;
}

void Movable::rotate(const Quaternion& rot_w)
{
	_orientation = rot_w * _orientation;
	_isDirty = true;
}