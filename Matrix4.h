#ifndef FROST_MATRIX_4
#define FROST_MATRIX_4

/*****************************************************\

		Matrix4: a 4x4 matrix

This class holds information for a 4x4 matrix, with the
  last row always being equal to (0, 0, 0, 1).
This is used for transformation matrices exclusively.
  Optimizations have been added such that using this
  class for a regular 4x4 matrix would be a bad idea.

\*****************************************************/

#include "Vect3.h"
#include "Quaternion.h"

namespace Frost {
	class Matrix4 {
	public:
		// Default ctor - set as identity matrix
		Matrix4();

		// Build matrix from _(row)(col) values
		Matrix4(float _11, float _12, float _13, float _14, float _21, float _22, float _23, float _24, float _31, float _32, float _33, float _34);

		// Transforms a vector using matrix multiplication
		Vect3 operator*(const Vect3& vect) const;

		// Returns the product of this matrix and
		//  other provided matrix o
		Matrix4 operator*(const Matrix4 &o) const;

		// Get determinant of the matrix
		float getDeterminant() const;

		// Set this to the inverse of the matrix provided
		void setInverse(const Matrix4& m);

		// Returns the inverse of this matrix
		Matrix4 inverse() const;

		// Build transformation matrix from a rotation and translation
		void setOrientationAndPos(const Quaternion& q, const Vect3& pos);

		// Invert this matrix
		void invert();

		// Transformations
		Vect3 transform(const Vect3& vect) const;
		Vect3 transformInverse(const Vect3& vect) const;

		// Transform a direction
		Vect3 transformDirection(const Vect3& vector) const;
		Vect3 transformInverseDirection(const Vect3& vector) const;

		float data[12];
	};

	// Translate a point from local space to world space, using provided
	//  transform matrix.
	Vect3 localToWorld(const Vect3 & local, const Matrix4& transform);

	// Transform a point from world space to local space, using the provided
	//  transform matrix.
	Vect3 worldToLocal(const Vect3 & world, const Matrix4& transform);

	// Transform a direction from local space to world space, using the provided
	//  transform matrix. Only applies rotation and scaling - no translation.
	Vect3 localToWorldDirn(const Vect3& local, const Matrix4& transform);

	// Transform a direction from world to local space, using provided transform
	//  matrix. Only applies scaling and rotation - no translation.
	Vect3 worldToLocalDirn(const Vect3& world, const Matrix4& transform);
}

#endif
