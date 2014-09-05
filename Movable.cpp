#include "Movable.h"
using namespace Frost;

Movable::Movable()
: _pos(0.f, 0.f, 0.f)
, _orientation(MathConstants::QUATERNION_UNIT)
{}

Movable::Movable(const Movable& toCopy)
: _pos(toCopy._pos)
, _orientation(toCopy._orientation)
{}

Movable::Movable(const FLOAT3& pos)
: _pos(pos)
, _orientation(MathConstants::QUATERNION_UNIT)
{}

Movable::Movable(const FLOAT3& pos, const Quaternion& orientation)
: _pos(pos)
, _orientation(orientation)
{}

//////////////// GETTERS/SETTERS /////////////////

// Gets the world position of the object...
Vect3 Movable::GetPos() const
{
	return _pos;
}

Quaternion Movable::GetOrientation() const
{
	return _orientation;
}

void Movable::SetPos(const Vect3& position)
{
	_pos = position;
}

void Movable::SetOrientation(const Quaternion& orientation)
{
	_orientation = orientation;
}

Matrix Movable::GetTransformMatrix() const
{
	return Matrix(_orientation, _pos);
}

//////////////// TRANSFORM OPS ////////////////////

void Movable::Move(const FLOAT3& moveAmount_w)
{
	_pos += moveAmount_w;
}

void Movable::Rotate(const Quaternion& rot_w)
{
	_orientation = rot_w * _orientation;
}