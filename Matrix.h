#ifndef _MATRIX_3_H_
#define _MATRIX_3_H_

// CURRENTLY THIS IS NOT COMPLETE.

#include "PhysicsClass.h"
#include "Quaternion.h"

namespace Frost {
	// Usually holds an inertia tensor
	class Matrix3 {
	public:
		Matrix3() {
			for(int i = 0; i < 9; i++) {
				data[i] = 0.0f;
			}
		}

		float data[9];

		Matrix3(float _11, float _12, float _13, float _21, float _22, float _23, float _31, float _32, float _33)
		{
			data[0] = _11;
			data[1] = _12;
			data[2] = _13;

			data[3] = _21;
			data[4] = _22;
			data[5] = _23;

			data[6] = _31;
			data[7] = _32;
			data[8] = _33;
		}

		// Matrix times a matrix
		Matrix3 operator*(const Matrix3 &o) const {
			return Matrix3(
				data[0]*o.data[0] + data[1]*o.data[3] + data[2]*o.data[6],
				data[0]*o.data[1] + data[1]*o.data[4] + data[2]*o.data[7],
				data[0]*o.data[2] + data[1]*o.data[5] + data[2]*o.data[8],

				data[3]*o.data[0] + data[4]*o.data[3] + data[5]*o.data[6],
				data[3]*o.data[1] + data[4]*o.data[4] + data[5]*o.data[7],
				data[3]*o.data[2] + data[4]*o.data[5] + data[5]*o.data[8],

				data[6]*o.data[0] + data[7]*o.data[3] + data[8]*o.data[6],
				data[6]*o.data[1] + data[7]*o.data[4] + data[8]*o.data[7],
				data[6]*o.data[2] + data[7]*o.data[5] + data[8]*o.data[8]
			);
		}

		// Set as the inverse of a matrix, using witchcraft
		void setInverse(const Matrix3& m) {
			float t4 = m.data[0]*m.data[4];
			float t6 = m.data[0]*m.data[5];
			float t8 = m.data[1]*m.data[3];
			float t10 = m.data[2]*m.data[3];
			float t12 = m.data[1]*m.data[6];
			float t14 = m.data[2]*m.data[6];

			// Calculate the determinant.
			float t16 = (t4*m.data[8] - t6*m.data[7] - t8*m.data[8] + t10*m.data[7] + t12*m.data[5] - t14*m.data[4]);
		
			// Make sure the determinant is non-zero.
			if (t16 == (float)0.0f)
				return;
			float t17 = 1/t16;
			data[0] = (m.data[4]*m.data[8]-m.data[5]*m.data[7])*t17;
			data[1] = -(m.data[1]*m.data[8]-m.data[2]*m.data[7])*t17;
			data[2] = (m.data[1]*m.data[5]-m.data[2]*m.data[4])*t17;

			data[3] = -(m.data[3]*m.data[8]-m.data[5]*m.data[6])*t17;
			data[4] = (m.data[0]*m.data[8]-t14)*t17;
			data[5] = -(t6-t10)*t17;

			data[6] = (m.data[3]*m.data[7]-m.data[4]*m.data[6])*t17;
			data[7] = -(m.data[0]*m.data[7]-t12)*t17;
			data[8] = (t4-t8)*t17;
		}

		Matrix3 inverse() const {
			Matrix3 toReturn;
			toReturn.setInverse(*this);
			return toReturn;
		}

		// Invert the matrix
		void invert() {
			setInverse(*this);
		}

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

	class Matrix4 {
	public:
		float data[12];

		Matrix4() {
			for(int i = 0; i < 12; i++)
				data[i] = 0.0f;
		}

		Matrix4(float _11, float _12, float _13, float _14, float _21, float _22, float _23, float _24, float _31, float _32, float _33, float _34) {
			data[0] = _11;
			data[1] = _12;
			data[2] = _13;
			data[3] = _14;

			data[4] = _21;
			data[5] = _22;
			data[6] = _23;
			data[7] = _24;

			data[8] = _31;
			data[9] = _32;
			data[10] = _33;
			data[11] = _34;
		}

		// Transforms a vector using matrix multiplication
		Vect3 operator*(const Vect3& vect) {
			return Vect3(
				vect.x * data[0] +
				vect.y * data[1] +
				vect.z * data[2] + data[3],

				vect.x * data[4] +
				vect.y * data[5] +
				vect.z * data[6] + data[7],

				vect.x * data[8] +
				vect.y * data[9] +
				vect.z * data[10] + data[11]);
		}

		// Returns a matrix that is this times another one.
		Matrix4 operator*(const Matrix4 &o) const {
			Matrix4 result;

			result.data[0] = (o.data[0]*data[0]) + (o.data[4]*data[1]) + (o.data[8]*data[2]);
			result.data[4] = (o.data[0]*data[4]) + (o.data[4]*data[5]) + (o.data[8]*data[6]);
			result.data[8] = (o.data[0]*data[8]) + (o.data[4]*data[9]) + (o.data[8]*data[10]);

			result.data[1] = (o.data[1]*data[0]) + (o.data[5]*data[1]) + (o.data[9]*data[2]);
			result.data[5] = (o.data[1]*data[4]) + (o.data[5]*data[5]) + (o.data[9]*data[6]);
			result.data[9] = (o.data[1]*data[8]) + (o.data[5]*data[9]) + (o.data[9]*data[10]);

			result.data[2] = (o.data[2]*data[0]) + (o.data[6]*data[1]) + (o.data[10]*data[2]);
			result.data[6] = (o.data[2]*data[4]) + (o.data[6]*data[5]) + (o.data[10]*data[6]);
			result.data[10] = (o.data[2]*data[8]) + (o.data[6]*data[9]) + (o.data[10]*data[10]);

			result.data[3] = (o.data[3]*data[0]) + (o.data[7]*data[1]) + (o.data[11]*data[2]) + data[3];
			result.data[7] = (o.data[3]*data[4]) + (o.data[7]*data[5]) + (o.data[11]*data[6]) + data[7];
			result.data[11] = (o.data[3]*data[8]) + (o.data[7]*data[9]) + (o.data[11]*data[10]) + data[11];

			return result;
		}

		// Get determinant of the matrix
		float getDeterminant() const {
			return data[8] * data[5] * data[2] +
				data[4] * data[9] * data[2] +
				data[8] * data[1] * data[6] -
				data[0] * data[9] * data[6] -
				data[4] * data[1] * data[10] +
				data[0] * data[5] * data[10];
		}

		// Set Inverse
		void setInverse(const Matrix4& m) {
			// Make sure the determinant is non-zero.
			float det = getDeterminant();
			if (det == 0)
				return;
			det = ((float)1.0)/det;

			data[0] = (-m.data[9]*m.data[6]+m.data[5]*m.data[10])*det;
			data[4] = (m.data[8]*m.data[6]-m.data[4]*m.data[10])*det;
			data[8] = (-m.data[8]*m.data[5]+m.data[4]*m.data[9]* m.data[15])*det;

			data[1] = (m.data[9]*m.data[2]-m.data[1]*m.data[10])*det;
			data[5] = (-m.data[8]*m.data[2]+m.data[0]*m.data[10])*det;
			data[9] = (m.data[8]*m.data[1]-m.data[0]*m.data[9]* m.data[15])*det;

			data[2] = (-m.data[5]*m.data[2]+m.data[1]*m.data[6]* m.data[15])*det;
			data[6] = (+m.data[4]*m.data[2]-m.data[0]*m.data[6]* m.data[15])*det;
			data[10] = (-m.data[4]*m.data[1]+m.data[0]*m.data[5]* m.data[15])*det;

			data[3] = (m.data[9]*m.data[6]*m.data[3] -
				m.data[5]*m.data[10]*m.data[3] -
				m.data[9]*m.data[2]*m.data[7] +
				m.data[1]*m.data[10]*m.data[7] +
				m.data[5]*m.data[2]*m.data[11] -
				m.data[1]*m.data[6]*m.data[11])*det;
			data[7] = (-m.data[8]*m.data[6]*m.data[3] +
				m.data[4]*m.data[10]*m.data[3] +
				m.data[8]*m.data[2]*m.data[7] -
				m.data[0]*m.data[10]*m.data[7] -
				m.data[4]*m.data[2]*m.data[11] +
				m.data[0]*m.data[6]*m.data[11])*det;
			data[11] =(m.data[8]*m.data[5]*m.data[3] -
				m.data[4]*m.data[9]*m.data[3] -
				m.data[8]*m.data[1]*m.data[7] +
				m.data[0]*m.data[9]*m.data[7] +
				m.data[4]*m.data[1]*m.data[11] -
				m.data[0]*m.data[5]*m.data[11])*det;
		}

		// Return new matrix with inverse
		Matrix4 inverse() const;

		void invert() {
			setInverse(*this);
		}

		// Set to be rotation matrix based on quaternion
		void setOrientationAndPos(const Quaternion& q, const Vect3& pos) {
			data[0] = 1 - (2*q.j*q.j + 2*q.k*q.k);
			data[1] = 2*q.i*q.j + 2*q.k*q.r;
			data[2] = 2*q.i*q.k - 2*q.j*q.r;
			data[3] = pos.x;

			data[4] = 2*q.i*q.j - 2*q.k*q.r;
			data[5] = 1 - (2*q.i*q.i + 2*q.k*q.k);
			data[6] = 2*q.j*q.k + 2*q.i*q.r;
			data[7] = pos.y;

			data[8] = 2*q.i*q.k + 2*q.j*q.r;
			data[9] = 2*q.j*q.k - 2*q.i*q.r;
			data[10] = 1 - (2*q.i*q.i + 2*q.j*q.j);
			data[11] = pos.z;
		}
	};

	Vect3 localToWorld(const Vect3 & local, const Matrix4& transform) {
		return transform * local;
		// Is this even right?
	}

	Vect3 worldToLocal(const Vect3 & world, const Matrix4& transform) {
		Matrix4 inverseTransform;
		inverseTransform.setInverse(transform);

		// I think this is right... Better test it.
		return inverseTransform * world;
	}

}
#endif
