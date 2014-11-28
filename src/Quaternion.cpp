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
#include "Quaternion.h"
#include <cmath>
using namespace Frost;

Quaternion::Quaternion()
{
	_x = 0.f;
	_y = 0.f;
	_z = 0.f;
	_w = 1.f;
}

Quaternion::Quaternion(float r, float i, float j, float k)
{
	_x = i;
	_y = j;
	_z = k;
	_w = r;
	float d = magnitude();

	_x = i / d;
	_y = j / d;
	_z = k / d;
	_w = r / d;
}

Quaternion::Quaternion(const FLOAT4& o)
{
	this->_x = o._x;
	this->_y = o._y;
	this->_z = o._z;
	this->_w = o._w;
	float d = magnitude();

	_x /= d;
	_y /= d;
	_z /= d;
	_w /= d;
}

Quaternion::Quaternion(const FLOAT3& axis, float angle)
{
	// Use the definition of a rotation quaternion...
	Vect3Normal axis_n(axis);
	angle /= 2.f;
	_w = std::cos(angle);
	_x = axis_n.x() * std::sin(angle);
	_y = axis_n.y() * std::sin(angle);
	_z = axis_n.z() * std::sin(angle);
}

///////////// OPERATOR OVERLOADS /////////////
Quaternion& Quaternion::operator=(const FLOAT4& o)
{
	float mag = magnitude();
	this->_x = o._x / mag;
	this->_y = o._y / mag;
	this->_z = o._z / mag;
	this->_w = o._w / mag;
	return *this;
}

Quaternion& Quaternion::operator=(const Quaternion& o)
{
	// Since this is also a quaternion, we're guaranteed that it's normal.
	this->_x = o._x;
	this->_y = o._y;
	this->_z = o._z;
	this->_w = o._w;
	return *this;
}

Quaternion Quaternion::operator*(const FLOAT4& o) const
{
	return Quaternion(
		this->_w * o._w - this->_x * o._x - this->_y * o._y - this->_z * o._z,
		this->_w * o._x + this->_x * o._w + this->_y * o._z - this->_z * o._y,
		this->_w * o._y + this->_y * o._w + this->_z * o._x - this->_x * o._z,
		this->_w * o._z + this->_z * o._w + this->_x * o._y - this->_y * o._x);
}

Quaternion& Quaternion::operator*=(const FLOAT4& other)
{
	*this = *this * other;
	return *this;
}

Quaternion& Quaternion::operator+=(const FLOAT3& other)
{
	if (other._x == 0.f && other._y == 0.f && other._z == 0.f) return *this;

	FLOAT4 q = { 0.f, other._x, other._y, other._z };

	q._w = -this->_x * other._x - this->_y * other._y - this->_z * other._z;
	q._x = this->_w * other._x + this->_y * other._z - this->_z * other._y;
	q._y = this->_w * other._y + this->_z * other._x - this->_x * other._z;
	q._z = this->_w * other._z + this->_x * other._y - this->_y * other._x;
//	q *= *this;

	_w += q._w * 0.5f;
	_x += q._x * 0.5f;
	_y += q._y * 0.5f;
	_z += q._z * 0.5f;

	float mag = this->magnitude();
	_w /= mag;
	_x /= mag;
	_y /= mag;
	_z /= mag;

	return *this;
}

Quaternion Quaternion::operator+(const FLOAT3& other) const
{
	if (other._x == 0.f && other._y == 0.f && other._z == 0.f) return *this;

	Quaternion toReturn(*this);

	FLOAT4 q = { 0.f, other._x, other._y, other._z };

	q._w = -this->_x * other._x - this->_y * other._y - this->_z * other._z;
	q._x = this->_w * other._x + this->_y * other._z - this->_z * other._y;
	q._y = this->_w * other._y + this->_z * other._x - this->_x * other._z;
	q._z = this->_w * other._z + this->_x * other._y - this->_y * other._x;
	//	q *= *this;

	toReturn._w += q._w * 0.5f;
	toReturn._x += q._x * 0.5f;
	toReturn._y += q._y * 0.5f;
	toReturn._z += q._z * 0.5f;

	float mag = toReturn.magnitude();
	toReturn._w /= mag;
	toReturn._x /= mag;
	toReturn._y /= mag;
	toReturn._z /= mag;

	return toReturn;
}

/////////////// QUATERNION FUNCS /////////////
void Quaternion::getAxisAngle(FLOAT3& o_Axis, float& o_Angle) const
{
	o_Angle = 2.f * std::acos(_w);
	float sin_half_angle = std::sin(o_Angle / 2.f);
	if (sin_half_angle == 0.f)
	{
		o_Axis = Vect3(1.f, 0.f, 0.f);
	}
	else
	{
		o_Axis = Vect3Normal(
			this->_x / sin_half_angle,
			this->_y / sin_half_angle,
			this->_z / sin_half_angle).asVect3();
	}
}

inline float Quaternion::magnitude()
{
	float toReturn = std::sqrt(_x * _x + _y * _y + _z * _z + _w * _w);
	if (toReturn == 0.f) throw ZeroMagnitudeException();
	return toReturn;
}
