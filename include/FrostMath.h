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
#ifndef FROST_MATH_COLLECTION_H
#define FROST_MATH_COLLECTION_H

#include "FloatStructs.h"
#include "FrostExceptions.h"
#include "Matrix.h"
#include "Quaternion.h"
#include "Vect3.h"
#include "Vect3Normal.h"

namespace Frost
{
	namespace MathConstants
	{
		// SCALARS
		const float PI = 3.141592654f;
		const float PI_DIV_2 = 1.5707963265f;
		const float PI_DIV_3 = 1.047197551f;
		const float PI_DIV_4 = 0.78539816325f;
		const float PI_MULT_2 = 6.283185306f;
		const float DEG_TO_RAD = PI / 180.f;
		const float RAD_TO_DEG = 1.f / DEG_TO_RAD;

		// MATRICES
		const Matrix MATRIX_IDENTITY = Matrix();
		const Matrix MATRIX_ZERO = Matrix(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f);

		// VECT3
		const Vect3 VECTOR_ZERO = Vect3(0.f, 0.f, 0.f);
		const Vect3 VECTOR_UNIT_X = Vect3(1.f, 0.f, 0.f);
		const Vect3 VECTOR_UNIT_Y = Vect3(0.f, 1.f, 0.f);
		const Vect3 VECTOR_UNIT_Z = Vect3(0.f, 0.f, 1.f);
		const Vect3 VECTOR_ONE = Vect3(1.f, 1.f, 1.f);

		// QUATERNION
		const Quaternion QUATERNION_UNIT = Quaternion();
	}
}

#endif