#include "Vect3Normal.h"
using namespace Frost;

/////////////////// CTORS //////////////////////
Vect3Normal::Vect3Normal() : Vect3(1.f, 0.f, 0.f) {}
Vect3Normal::Vect3Normal(float x, float y, float z)
: Vect3(x, y, z)
{
	float mag = this->Magnitude();
	if (0.f == mag) throw ZeroMagnitudeException();
	_x /= mag;
	_y /= mag;
	_z /= mag;
}

Vect3Normal::Vect3Normal(const FLOAT3& o)
: Vect3(o)
{
	float mag = this->Magnitude();
	if (0.f == mag) throw ZeroMagnitudeException();
	_x /= mag;
	_y /= mag;
	_z /= mag;
}

Vect3Normal::Vect3Normal(const Vect3& o)
: Vect3(o)
{
	float mag = this->Magnitude();
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
	float mag = other.Magnitude();
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