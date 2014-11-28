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
// Vect3: A 3-dimensional vector class,
//  supporting standard vector math funcs
/////////////////////////////////////////

#ifndef FROST_MATH_VECT3_H
#define FROST_MATH_VECT3_H

#include "FloatStructs.h"
#include "FrostExceptions.h"
#include "DebugLogger.h"

namespace Frost
{
	class Vect3 : public FLOAT3
	{
	public:
		/////////////////// CTORS //////////////////////
		
		// Default ctor: initialize to zero vector
		Vect3();
		// Initialize component-wise
		Vect3(float x, float y, float z);
		// Initialize with another Vect3 or other R32B32G32_FLOAT type object
		Vect3(const FLOAT3& right);
		// Initialize with another Vect3 or other R32B32G32_FLOAT type object
		Vect3(const Vect3& right);

		///////////// OPERATOR OVERLOADS /////////////
		Vect3 operator+(const FLOAT3& other) const;
		Vect3 operator-(const FLOAT3& other) const;
		float operator*(const FLOAT3& other) const;
		Vect3 operator%(const FLOAT3& other) const;
		Vect3 operator*(float other) const;
		Vect3 operator/(float other) const;
		Vect3& operator+=(const FLOAT3& other);
		Vect3& operator-=(const FLOAT3& other);
		Vect3& operator*=(float other);
		Vect3& operator/=(float other);
		float operator[](int i) const;
		float& operator[](int i);
		Vect3& operator=(const FLOAT3& other);
		bool operator==(const FLOAT3& other) const;
		bool operator!=(const FLOAT3& other) const;

		///////////// OTHER VECTOR FUNCTIONS //////////////
		float magnitude() const;
		float squareMagnitude() const;
		bool isApproximately(const FLOAT3& other, float tolerableError);
		bool isApproximately(const FLOAT3& other, FLOAT3 tolerableError);

	private:
	};

	// Perform a dot product operation on the two specified vectors
	float DotProduct(const Vect3& v1, const Vect3& v2);

	// Perform a cross product operation on the two specified vectors
	Vect3 CrossProduct(const Vect3& v1, const Vect3& v2);

	// Perform a component-wise multiplication on the two specified vectors
	Vect3 ComponentProduct(const Vect3& v1, const Vect3& v2);

	Vect3 operator*(float n, const Vect3& o);

	std::ostream& operator<<(std::ostream& o, FLOAT3 v) {
		return o << "<" << v._x << ", " << v._y << ", " << v._z << ">";
	}

	std::ostream& operator<<(std::ostream& o, Vect3 v) {
		return o << "<" << v._x << ", " << v._y << ", " << v._z << ">";
	}
}

#endif
