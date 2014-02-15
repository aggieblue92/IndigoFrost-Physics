#include "Matrix3.h"
using namespace Frost;

Matrix3::Matrix3() {
	for(int i = 0; i < 9; i++) {
		data[i] = 0.0f;
	}
}

Matrix3::Matrix3(float _11, float _12, float _13, float _21, float _22, float _23, float _31, float _32, float _33)
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

Matrix3 Matrix3::operator*(const Matrix3& o) const {
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

void Matrix3::setInverse(const Matrix3& m) {
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

Matrix3 Matrix3::inverse() const {
	Matrix3 toReturn;
	toReturn.setInverse(*this);
	return toReturn;
}

void Matrix3::invert() {
	setInverse(*this);
}