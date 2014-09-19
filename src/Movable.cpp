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
	_isDirty = true;
}

void Movable::SetOrientation(const Quaternion& orientation)
{
	_orientation = orientation;
	_isDirty = true;
}

Matrix Movable::GetTransformMatrix()
{
	if (_isDirty)
	{
		_transformMatrix = Matrix(_orientation, _pos);
		_isDirty = false;
	}

	return _transformMatrix;
}

Matrix Movable::GetTransformMatrix() const
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

void Movable::Move(const FLOAT3& moveAmount_w)
{
	_pos += moveAmount_w;
	_isDirty = true;
}

void Movable::Rotate(const Quaternion& rot_w)
{
	_orientation = rot_w * _orientation;
	_isDirty = true;
}