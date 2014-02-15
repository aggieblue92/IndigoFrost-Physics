#include "Matrix4.h"
using namespace Frost;

Matrix4::Matrix4() {
	for(int i = 0; i < 12; i++)
		data[i] = 0.0f;
}

Matrix4::Matrix4(float _11, float _12, float _13, float _14, float _21, float _22, float _23, float _24, float _31, float _32, float _33, float _34) {
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

Matrix4 Matrix4::operator*(const Matrix4& o) const {
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

void Matrix4::setInverse(const Matrix4& m) {
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

Vect3 Matrix4::operator*(const Vect3& vect) {
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

float Matrix4::getDeterminant() const {
	return data[8] * data[5] * data[2] +
		data[4] * data[9] * data[2] +
		data[8] * data[1] * data[6] -
		data[0] * data[9] * data[6] -
		data[4] * data[1] * data[10] +
		data[0] * data[5] * data[10];
}

void Matrix4::setOrientationAndPos(const Quaternion& q, const Vect3& pos) {
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

void Matrix4::invert() {
	setInverse(*this);
}

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