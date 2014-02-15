#ifndef FROST_MATRIX_4
#define FROST_MATRIX_4

#include "PhysicsClass.h"
#include "Quaternion.h"

namespace Frost {
	class Matrix4 {
	public:
		Matrix4();
		Matrix4(float _11, float _12, float _13, float _14, float _21, float _22, float _23, float _24, float _31, float _32, float _33, float _34);
		// Transforms a vector using matrix multiplication
		Vect3 operator*(const Vect3& vect);
		// Returns a matrix that is this times another one.
		Matrix4 operator*(const Matrix4 &o) const;
		// Get determinant of the matrix
		float getDeterminant() const;
		// Set Inverse
		void setInverse(const Matrix4& m);
		// Return new matrix with inverse
		Matrix4 inverse() const;
		// Set to be rotation matrix based on quaternion
		void setOrientationAndPos(const Quaternion& q, const Vect3& pos);
		void invert();

		float data[12];
	};

	Vect3 localToWorld(const Vect3 & local, const Matrix4& transform);
	Vect3 worldToLocal(const Vect3 & world, const Matrix4& transform);
}

#endif
