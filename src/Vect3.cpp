#include "Vect3.h"
#include <cmath>
using namespace Frost;

Vect3::Vect3()
{
	_x = 0.f;
	_y = 0.f;
	_z = 0.f;
}

Vect3::Vect3(float x, float y, float z)
{
	_x = x;
	_y = y;
	_z = z;
}

Vect3::Vect3(const FLOAT3& o)
{
	_x = o._x;
	_y = o._y;
	_z = o._z;
}

Vect3::Vect3(const Vect3& o)
{
	_x = o._x;
	_y = o._y;
	_z = o._z;
}

///////////// OPERATOR OVERLOADS /////////////
Vect3 Vect3::operator+(const FLOAT3& other) const
{
	return Vect3(this->_x + other._x, this->_y + other._y, this->_z + other._z);
}

Vect3 Vect3::operator-(const FLOAT3& other) const
{
	return Vect3(this->_x - other._x, this->_y - other._y, this->_z - other._z);
}

float Vect3::operator*(const FLOAT3& o) const
{
	// Dot product...
	return this->_x * o._x + this->_y * o._y + this->_z * o._z;
}

Vect3 Vect3::operator%(const FLOAT3& o) const
{
	// Cross product...
	return Vect3
		(
		this->_y * o._z - this->_z * o._y,
		this->_x * o._z - this->_z * o._x,
		this->_x * o._y - this->_y * o._x
		);
}

Vect3 Vect3::operator*(float o) const
{
	return Vect3(this->_x * o, this->_y * o, this->_z * o);
}

Vect3 Vect3::operator/(float o) const
{
	if (o == 0.f) throw DivByZeroException();
	return Vect3(this->_x / o, this->_y / o, this->_z / o);
}

Vect3& Vect3::operator+=(const FLOAT3& o)
{
	this->_x += o._x;
	this->_y += o._y;
	this->_z += o._z;
	return *this;
}

Vect3& Vect3::operator-=(const FLOAT3& o)
{
	this->_x -= o._x;
	this->_y -= o._y;
	this->_z -= o._z;
	return *this;
}

Vect3& Vect3::operator*=(float o)
{
	this->_x *= o;
	this->_y *= o;
	this->_z *= o;
	return *this;
}

Vect3& Vect3::operator/=(float o)
{
	if (o == 0) throw DivByZeroException();
	this->_x /= o;
	this->_y /= o;
	this->_z /= o;
	return *this;
}

float Vect3::operator[](int i) const
{
	if (i < 0 || i > 2) throw IndexOutOfBoundsException(i);
	else return *(reinterpret_cast<const float*>(this) + i);
}

float& Vect3::operator[](int i)
{
	if (i < 0 || i > 2) throw IndexOutOfBoundsException(i);
	else return *(reinterpret_cast<float*>(this) + i);
}

Vect3& Vect3::operator=(const FLOAT3& o)
{
	if (this == &o) return *this;
	this->_x = o._x;
	this->_y = o._y;
	this->_z = o._z;
	return *this;
}

bool Vect3::operator==(const FLOAT3& o) const
{
	return
		this->_x == o._x &&
		this->_y == o._y &&
		this->_z == o._z;
}

///////////// OTHER VECTOR FUNCTIONS //////////////
float Vect3::Magnitude() const
{
	return std::sqrt(SquareMagnitude());
}

float Vect3::SquareMagnitude() const
{
	return _x * _x + _y * _y + _z * _z;
}

// Functions:
float Frost::DotProduct(const Vect3& v1, const Vect3& v2)
{
	return v1 * v2;
}

Vect3 Frost::CrossProduct(const Vect3& v1, const Vect3& v2)
{
	return v1 % v2;
}

Vect3 Frost::ComponentProduct(const Vect3& v1, const Vect3& v2)
{
	return Vect3(v1._x * v2._x, v1._y * v2._y, v1._z * v2._z);
}

Vect3 Frost::operator*(float n, const Vect3& o)
{
	return o * n;
}