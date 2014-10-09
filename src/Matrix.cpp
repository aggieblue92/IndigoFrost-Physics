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

#include "Matrix.h"
#include <cmath>
using namespace Frost;

Matrix::Matrix()
{
	// Create the identity matrix
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m[i][j] = (i == j ? 1.f : 0.f);
		}
	}
}

Matrix::Matrix(const Matrix& o)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m[i][j] = o.m[i][j];
		}
	}
}

Matrix::Matrix(const FLOAT4X4& o)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m[i][j] = o.m[i][j];
		}
	}
}

Matrix::Matrix(const FLOAT4& c1, const FLOAT4& c2, const FLOAT4& c3, const FLOAT4& c4)
{
	for (int colNum = 0; colNum < 4; colNum++)
	{
		m[0][colNum] = c1._x;
		m[1][colNum] = c1._y;
		m[2][colNum] = c1._z;
		m[3][colNum] = c1._w;
	}
}

Matrix::Matrix(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44)
{
	m[0][0] = m11; m[0][1] = m12; m[0][2] = m13; m[0][3] = m14;
	m[1][0] = m21; m[1][1] = m22; m[1][2] = m23; m[1][3] = m24;
	m[2][0] = m31; m[2][1] = m32; m[2][2] = m33; m[2][3] = m34;
	m[3][0] = m41; m[3][1] = m42; m[3][2] = m43; m[3][3] = m44;
}

Matrix::Matrix(const Quaternion& q)
{
	// This is black magic to me, but apparently it works.
	//  This algorithm comes from the dark reaches of the Internet.
	_11 = 1.f - (2.f * q._y * q._y + 2.f * q._z * q._z);
	_12 = 2.f * q._x * q._y + 2.f * q._z * q._w;
	_13 = 2.f * q._x * q._z - 2.f * q._y * q._w;
	_14 = 0.f;

	_21 = 2.f * q._x * q._y - 2.f * q._z * q._w;
	_22 = 1.f - (2.f * q._x * q._x + 2.f * q._z * q._z);
	_23 = 2.f * q._y * q._z + 2.f * q._x * q._w;
	_24 = 0.f;

	_31 = 2.f * q._x * q._z + 2.f * q._y * q._w;
	_32 = 2.f * q._y * q._z - 2.f * q._x * q._w;
	_33 = 1.f - (2.f * q._x * q._x + 2.f * q._y * q._y);
	_34 = 0.f;

	_41 = 0.f;
	_42 = 0.f;
	_43 = 0.f;
	_44 = 1.f;
}

Matrix::Matrix(const Vect3& t) : Matrix()
{
	this->_14 = t._x;
	this->_24 = t._y;
	this->_34 = t._z;
}

Matrix::Matrix(const Quaternion& rotation, const FLOAT3& t)
: Matrix(rotation)
{
	this->_14 = t._x;
	this->_24 = t._y;
	this->_34 = t._z;
}

///////////// OPERATOR OVERLOADS /////////////
Matrix& Matrix::operator=(const FLOAT4X4& o)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			this->m[i][j] = o.m[i][j];
		}
	}
	return *this;
}

Matrix Matrix::operator+(const FLOAT4X4& o) const
{
	Matrix toReturn(*this);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			toReturn.m[i][j] += o.m[i][j];
		}
	}
	return toReturn;
}

Matrix Matrix::operator-(const FLOAT4X4& o) const
{
	Matrix toReturn(*this);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			toReturn.m[i][j] -= o.m[i][j];
		}
	}
	return toReturn;
}

Matrix Matrix::operator*(const FLOAT4X4& o) const
{
	// Rows from *this, cols from other
	Matrix toReturn;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			toReturn.m[i][j] = 0.f;
			for (int k = 0; k < 4; k++)
			{
				toReturn.m[i][j] +=
					this->m[i][k] * o.m[k][j];
			}
		}
	}
	return toReturn;
}

Vect3 Matrix::operator*(const FLOAT3& o) const
{
	// Same principle, but this time with only a col to return...
	Vect3 toReturn;
	toReturn._x = _11 * o._x + _12 * o._y + _13 * o._z + _14;
	toReturn._y = _21 * o._x + _22 * o._y + _23 * o._z + _24;
	toReturn._z = _31 * o._x + _32 * o._y + _33 * o._z + _34;
	return toReturn;
}

Matrix Matrix::operator*(float o) const
{
	Matrix toReturn(*this);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			toReturn.m[i][j] *= o;
		}
	}
	return toReturn;
}

FLOAT4 Matrix::operator[](int i) const
{
	if (i < 0 || i > 3) throw IndexOutOfBoundsException(i);
	return rows[i];
}

FLOAT4& Matrix::operator[](int i)
{
	if (i < 0 || i > 3) throw IndexOutOfBoundsException(i);
	return rows[i];
}

bool Matrix::operator==(const FLOAT4X4& o) const
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (this->m[i][j] != o.m[i][j]) return false;
		}
	}
	return true;
}

//////////////// OTHER MATRIX FUNCTIONS ////////////////
Matrix Matrix::getInverse() const
{
	// Source: http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
	Matrix toReturn;
	float inv[4][4], det;

	inv[0][0] = m[1][1] * m[2][2] * m[3][3] -
		m[1][1] * m[2][3] * m[3][2] -
		m[2][1] * m[1][2] * m[3][3] +
		m[2][1] * m[1][3] * m[3][2] +
		m[3][1] * m[1][2] * m[2][3] -
		m[3][1] * m[1][3] * m[2][2];

	inv[1][0] = -m[1][0] * m[2][2] * m[3][3] +
		m[1][0] * m[2][3] * m[3][2] +
		m[2][0] * m[1][2] * m[3][3] -
		m[2][0] * m[1][3] * m[3][2] -
		m[3][0] * m[1][2] * m[2][3] +
		m[3][0] * m[1][3] * m[2][2];

	inv[2][0] = m[1][0] * m[2][1] * m[3][3] -
		m[1][0] * m[2][3] * m[3][1] -
		m[2][0] * m[1][1] * m[3][3] +
		m[2][0] * m[1][3] * m[3][1] +
		m[3][0] * m[1][1] * m[2][3] -
		m[3][0] * m[1][3] * m[2][1];

	inv[3][0] = -m[1][0] * m[2][1] * m[3][2] +
		m[1][0] * m[2][2] * m[3][1] +
		m[2][0] * m[1][1] * m[3][2] -
		m[2][0] * m[1][2] * m[3][1] -
		m[3][0] * m[1][1] * m[2][2] +
		m[3][0] * m[1][2] * m[2][1];

	inv[0][1] = -m[0][1] * m[2][2] * m[3][3] +
		m[0][1] * m[2][3] * m[3][2] +
		m[2][1] * m[0][2] * m[3][3] -
		m[2][1] * m[0][3] * m[3][2] -
		m[3][1] * m[0][2] * m[2][3] +
		m[3][1] * m[0][3] * m[2][2];

	inv[1][1] = m[0][0] * m[2][2] * m[3][3] -
		m[0][0] * m[2][3] * m[3][2] -
		m[2][0] * m[0][2] * m[3][3] +
		m[2][0] * m[0][3] * m[3][2] +
		m[3][0] * m[0][2] * m[2][3] -
		m[3][0] * m[0][3] * m[2][2];

	inv[2][1] = -m[0][0] * m[2][1] * m[3][3] +
		m[0][0] * m[2][3] * m[3][1] +
		m[2][0] * m[0][1] * m[3][3] -
		m[2][0] * m[0][3] * m[3][1] -
		m[3][0] * m[0][1] * m[2][3] +
		m[3][0] * m[0][3] * m[2][1];

	inv[3][1] = m[0][0] * m[2][1] * m[3][2] -
		m[0][0] * m[2][2] * m[3][1] -
		m[2][0] * m[0][1] * m[3][2] +
		m[2][0] * m[0][2] * m[3][1] +
		m[3][0] * m[0][1] * m[2][2] -
		m[3][0] * m[0][2] * m[2][1];

	inv[0][2] = m[0][1] * m[1][2] * m[3][3] -
		m[0][1] * m[1][3] * m[3][2] -
		m[1][1] * m[0][2] * m[3][3] +
		m[1][1] * m[0][3] * m[3][2] +
		m[3][1] * m[0][2] * m[1][3] -
		m[3][1] * m[0][3] * m[1][2];

	inv[1][2] = -m[0][0] * m[1][2] * m[3][3] +
		m[0][0] * m[1][3] * m[3][2] +
		m[1][0] * m[0][2] * m[3][3] -
		m[1][0] * m[0][3] * m[3][2] -
		m[3][0] * m[0][2] * m[1][3] +
		m[3][0] * m[0][3] * m[1][2];

	inv[2][2] = m[0][0] * m[1][1] * m[3][3] -
		m[0][0] * m[1][3] * m[3][1] -
		m[1][0] * m[0][1] * m[3][3] +
		m[1][0] * m[0][3] * m[3][1] +
		m[3][0] * m[0][1] * m[1][3] -
		m[3][0] * m[0][3] * m[1][1];

	inv[3][2] = -m[0][0] * m[1][1] * m[3][2] +
		m[0][0] * m[1][2] * m[3][1] +
		m[1][0] * m[0][1] * m[3][2] -
		m[1][0] * m[0][2] * m[3][1] -
		m[3][0] * m[0][1] * m[1][2] +
		m[3][0] * m[0][2] * m[1][1];

	inv[0][3] = -m[0][1] * m[1][2] * m[2][3] +
		m[0][1] * m[1][3] * m[2][2] +
		m[1][1] * m[0][2] * m[2][3] -
		m[1][1] * m[0][3] * m[2][2] -
		m[2][1] * m[0][2] * m[1][3] +
		m[2][1] * m[0][3] * m[1][2];

	inv[1][3] = m[0][0] * m[1][2] * m[2][3] -
		m[0][0] * m[1][3] * m[2][2] -
		m[1][0] * m[0][2] * m[2][3] +
		m[1][0] * m[0][3] * m[2][2] +
		m[2][0] * m[0][2] * m[1][3] -
		m[2][0] * m[0][3] * m[1][2];

	inv[2][3] = -m[0][0] * m[1][1] * m[2][3] +
		m[0][0] * m[1][3] * m[2][1] +
		m[1][0] * m[0][1] * m[2][3] -
		m[1][0] * m[0][3] * m[2][1] -
		m[2][0] * m[0][1] * m[1][3] +
		m[2][0] * m[0][3] * m[1][1];

	inv[3][3] = m[0][0] * m[1][1] * m[2][2] -
		m[0][0] * m[1][2] * m[2][1] -
		m[1][0] * m[0][1] * m[2][2] +
		m[1][0] * m[0][2] * m[2][1] +
		m[2][0] * m[0][1] * m[1][2] -
		m[2][0] * m[0][2] * m[1][1];

	det = m[0][0] * inv[0][0] + m[0][1] * inv[1][0] + m[0][2] * inv[2][0] + m[0][3] * inv[3][0];

	if (det == 0) throw SingularMatrixException();

	det = 1.f / det;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			toReturn.m[i][j] = inv[i][j] * det;
		}
	}

	return toReturn;
}

Matrix Matrix::getTranspose() const
{
	Matrix toReturn;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			toReturn.m[i][j] = this->m[j][i];
		}
	}
	return toReturn;
}

Vect3 Matrix::transformCoord(const FLOAT3& o) const
{
	return Vect3(
		_11 * o._x + _12 * o._y + _13 * o._z + _14,
		_21 * o._x + _22 * o._y + _23 * o._z + _24,
		_31 * o._x + _32 * o._y + _33 * o._z + _34
		);
}

Vect3 Matrix::transformDirn(const FLOAT3& o) const
{
	return Vect3(
		_11 * o._x + _12 * o._y + _13 * o._z,
		_21 * o._x + _22 * o._y + _23 * o._z,
		_31 * o._x + _32 * o._y + _33 * o._z
		);
}

Vect3Normal Matrix::transformNormal(const FLOAT3& o) const
{
	return Vect3Normal(
		_11 * o._x + _12 * o._y + _13 * o._z,
		_21 * o._x + _22 * o._y + _23 * o._z,
		_31 * o._x + _32 * o._y + _33 * o._z
		);
}

void Matrix::getOrientationAndPosition(Vect3& o_pos, Quaternion& o_orientation) const
{
	o_pos = getPosition();
	o_orientation = getOrientation();
}

Quaternion Matrix::getOrientation() const
{
	// Orientation algorithm taken from http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/index.htm
	float trace = _11 + _22 + _33;

	if (trace > 0.f)
	{
		float S = 0.5f / std::sqrt(trace + 1.f);
		return Quaternion(
			0.25f / S,
			(_23 - _32) * S,
			(_31 - _13) * S,
			(_12 - _21) * S
			);
	}
	else if ((_11 > _22) && (_11 > _33))
	{
		float S = std::sqrt(1.f + _11 - _22 - _33) * 2.f;
		return Quaternion(
			(_23 - _32) / S,
			0.25f * S,
			(_21 + _12) / S,
			(_31 + _13) / S
			);
	}
	else if (_22 > _33)
	{
		float S = std::sqrt(1.f + _22 - _11 - _33) * 2.f;
		return Quaternion(
			(_31 - _13) / S,
			(_21 + _12) / S,
			0.25f * S,
			(_32 + _23) / S
			);
	}
	else
	{
		float S = std::sqrt(1.f + _33 - _11 - _22) * 2.f;
		return Quaternion(
			(_12 - _21) / S,
			(_31 + _13) / S,
			(_32 + _23) / S,
			0.25f * S
			);
	}
}

Vect3 Matrix::getPosition() const
{
	return Vect3(_14, _24, _34);
}