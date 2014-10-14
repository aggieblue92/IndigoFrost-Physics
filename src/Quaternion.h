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
/////////////////////////////////////////
// Quaternion: A 4-d vector class, with
//  all the additional bells and whistles
//  of quaternion math
/////////////////////////////////////////

#ifndef FROST_QUATERNION_H
#define FROST_QUATERNION_H

#include "FloatStructs.h"
#include "FrostExceptions.h"
#include "Vect3Normal.h"

namespace Frost
{
	class Quaternion : public FLOAT4
	{
	public:
		/////////////////// CTORS //////////////////////

		// Default ctor - initialize with no rotation
		Quaternion();

		// Initialize values manually
		Quaternion(float r, float i, float j, float k);

		// Initialize from another R32G32B32A32_FLOAT structure
		Quaternion(const FLOAT4& other);

		// Initialize from an axis and an angle
		Quaternion(const FLOAT3& axis, float angle);

		///////////// OPERATOR OVERLOADS /////////////
		Quaternion& operator=(const FLOAT4& other);
		Quaternion& operator=(const Quaternion& other);
		Quaternion operator*(const FLOAT4& other) const;
		Quaternion& operator*=(const FLOAT4& other);
		Quaternion operator+(const FLOAT4& o) const
		{
			return Quaternion(
				this->_w + o._w,
				this->_x + o._x,
				this->_y + o._y,
				this->_z + o._z
				);
		}
		Quaternion& operator+=(const FLOAT3& other);
		Quaternion operator+(const FLOAT3& other) const;

		////////////////// QUATERNION FUNCS //////////////////
		void getAxisAngle(FLOAT3& o_Axis, float& o_Angle) const;
	private:
		float magnitude();
	};
}

#endif