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