#ifndef FROST_MATRIX_3_H
#define FROST_MATRIX_3_H

// CURRENTLY THIS IS NOT COMPLETE.

#include "PhysicsClass.h"
#include "Quaternion.h"

namespace Frost {
	// Usually holds an inertia tensor
	class Matrix3 {
	public:
		Matrix3();
		Matrix3(float _11, float _12, float _13, float _21, float _22, float _23, float _31, float _32, float _33);

		// Matrix multiplication
		Matrix3 operator*(const Matrix3 &o) const;
		// Set as the inverse of a matrix, using witchcraft
		void Matrix3::setInverse(const Matrix3& m);
		Matrix3 inverse() const;

		float data[9];

		// Invert the matrix
		void invert();

		// Set transpose...
		void setTranspose(const Matrix3& m) {
			data[0] = m.data[0];
			data[1] = m.data[3];
			data[2] = m.data[6];

			data[3] = m.data[1];
			data[4] = m.data[4];
			data[5] = m.data[7];

			data[6] = m.data[2];
			data[7] = m.data[5];
			data[8] = m.data[8];
		}

		// Returns a new matrix containing transpose of this matrix
		Matrix3 transpose() const {
			Matrix3 toReturn;
			toReturn.setTranspose(*this);
			return toReturn;
		}

		// Set orientation from a given quaternion
		void setOrientation(const Quaternion & q) {
			data[0] = 1 - (2*q.j*q.j + 2*q.k*q.k);
			data[1] = 2*q.i*q.j + 2*q.k*q.r;
			data[2] = 2*q.i*q.k - 2*q.j*q.r;
			data[3] = 2*q.i*q.j - 2*q.k*q.r;

			data[4] = 1 - (2*q.i*q.i + 2*q.k*q.k);
			data[5] = 2*q.j*q.k + 2*q.i*q.r;
			data[6] = 2*q.i*q.k + 2*q.j*q.r;
			data[7] = 2*q.j*q.k - 2*q.i*q.r;
			
			data[8] = 1 - (2*q.i*q.i + 2*q.j*q.j);
		}
	};
}
#endif
