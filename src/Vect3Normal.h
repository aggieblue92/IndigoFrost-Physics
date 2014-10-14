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
// Vect3Normal: Implementation of vector
//  class that enforces normality on read.
/////////////////////////////////////////

#ifndef FROST_MATH_VECT3_NORMAL_H
#define FROST_MATH_VECT3_NORMAL_H

#include "Vect3.h"

namespace Frost
{
	// Exceptions...
	class Vect3Normal : public Vect3
	{
	public:
		/////////////////// CTORS //////////////////////
		// Default ctor - initialize to the unit X vector
		Vect3Normal();
		// Initialize component-wise
		Vect3Normal(float x, float y, float z);
		// Initialize with another Vect3 or other R32B32G32_FLOAT structure
		Vect3Normal(const FLOAT3& right);
		// Initialize with another Vect3
		Vect3Normal(const Vect3& right);
		// Initialize with another normal vector (copy ctor)
		Vect3Normal(const Vect3Normal& right);

		///////////// OPERATOR OVERLOADS /////////////
		Vect3Normal& operator=(const FLOAT3& other);
		Vect3Normal& operator=(const Vect3Normal& other);

	private:
		// Define invalid operations here

		// FORBIDDEN!
		void operator+=(const FLOAT3&) {}
		// FORBIDDEN!
		void operator/=(float) {}
		// FORBIDDEN!
		void operator*=(float) {}
		// FORBIDDEN!
		void operator-=(const FLOAT3&) {}
	};
}

#endif
