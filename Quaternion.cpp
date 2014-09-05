#include "Quaternion.h"
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
	float d = Magnitude();

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
	float d = Magnitude();

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
	_w = std::cosf(angle);
	_x = axis_n._x * std::sinf(angle);
	_y = axis_n._y * std::sinf(angle);
	_z = axis_n._z * std::sinf(angle);
}

///////////// OPERATOR OVERLOADS /////////////
Quaternion& Quaternion::operator=(const FLOAT4& o)
{
	float mag = Magnitude();
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


/////////////// QUATERNION FUNCS /////////////
void Quaternion::GetAxisAngle(FLOAT3& o_Axis, float& o_Angle) const
{
	o_Angle = 2.f * std::acosf(_w);
	float sin_half_angle = std::sin(o_Angle / 2.f);
	o_Axis = Vect3Normal(
		this->_x / sin_half_angle,
		this->_y / sin_half_angle,
		this->_z / sin_half_angle);
}

inline float Quaternion::Magnitude()
{
	float toReturn = std::sqrt(_x * _x + _y * _y + _z * _z + _w * _w);
	if (toReturn == 0.f) throw ZeroMagnitudeException();
	return toReturn;
}