#include "Matrix4.h"
using namespace Frost;

Matrix4::Matrix4() {
	for(int i = 0; i < 12; i++)
		data[i] = 0.0f;
	data[0] = 1.f;
	data[5] = 1.f;
	data[10] = 1.f;
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

	// I found this code online, not gonna lie. Thank you, random
	//  StackExchange citizen! You saved me tons of time coding this
	//  ridiculous function!
	float inv[16], det;
	inv[0] = m.data[5]  * m.data[10] * 1.f - 
             m.data[5]  * m.data[11] * 0.f - 
             m.data[9]  * m.data[6]  * 1.f + 
             m.data[9]  * m.data[7]  * 0.f +
             0.f * m.data[6]  * m.data[11] - 
             0.f * m.data[7]  * m.data[10];

    inv[4] = -m.data[4]  * m.data[10] * 1.f + 
              m.data[4]  * m.data[11] * 0.f + 
              m.data[8]  * m.data[6]  * 1.f - 
              m.data[8]  * m.data[7]  * 0.f - 
              0.f * m.data[6]  * m.data[11] + 
              0.f * m.data[7]  * m.data[10];

    inv[8] = m.data[4]  * m.data[9] * 1.f - 
             m.data[4]  * m.data[11] * 0.f - 
             m.data[8]  * m.data[5] * 1.f + 
             m.data[8]  * m.data[7] * 0.f + 
             0.f * m.data[5] * m.data[11] - 
             0.f * m.data[7] * m.data[9];

    inv[12] = -m.data[4]  * m.data[9] * 0.f + 
               m.data[4]  * m.data[10] * 0.f +
               m.data[8]  * m.data[5] * 0.f - 
               m.data[8]  * m.data[6] * 0.f - 
               0.f * m.data[5] * m.data[10] + 
               0.f * m.data[6] * m.data[9];

    inv[1] = -m.data[1]  * m.data[10] * 1.f + 
              m.data[1]  * m.data[11] * 0.f + 
              m.data[9]  * m.data[2] * 1.f - 
              m.data[9]  * m.data[3] * 0.f - 
              0.f * m.data[2] * m.data[11] + 
              0.f * m.data[3] * m.data[10];

    inv[5] = m.data[0]  * m.data[10] * 1.f - 
             m.data[0]  * m.data[11] * 0.f - 
             m.data[8]  * m.data[2] * 1.f + 
             m.data[8]  * m.data[3] * 0.f + 
             0.f * m.data[2] * m.data[11] - 
             0.f * m.data[3] * m.data[10];

    inv[9] = -m.data[0]  * m.data[9] * 1.f + 
              m.data[0]  * m.data[11] * 0.f + 
              m.data[8]  * m.data[1] * 1.f - 
              m.data[8]  * m.data[3] * 0.f - 
              0.f * m.data[1] * m.data[11] + 
              0.f * m.data[3] * m.data[9];

    inv[13] = m.data[0]  * m.data[9] * 0.f - 
              m.data[0]  * m.data[10] * 0.f - 
              m.data[8]  * m.data[1] * 0.f + 
              m.data[8]  * m.data[2] * 0.f + 
              0.f * m.data[1] * m.data[10] - 
              0.f * m.data[2] * m.data[9];

    inv[2] = m.data[1]  * m.data[6] * 1.f - 
             m.data[1]  * m.data[7] * 0.f - 
             m.data[5]  * m.data[2] * 1.f + 
             m.data[5]  * m.data[3] * 0.f + 
             0.f * m.data[2] * m.data[7] - 
             0.f * m.data[3] * m.data[6];

    inv[6] = -m.data[0]  * m.data[6] * 1.f + 
              m.data[0]  * m.data[7] * 0.f + 
              m.data[4]  * m.data[2] * 1.f - 
              m.data[4]  * m.data[3] * 0.f - 
              0.f * m.data[2] * m.data[7] + 
              0.f * m.data[3] * m.data[6];

    inv[10] = m.data[0]  * m.data[5] * 1.f - 
              m.data[0]  * m.data[7] * 0.f - 
              m.data[4]  * m.data[1] * 1.f + 
              m.data[4]  * m.data[3] * 0.f + 
              0.f * m.data[1] * m.data[7] - 
              0.f * m.data[3] * m.data[5];

    inv[14] = -m.data[0]  * m.data[5] * 0.f + 
               m.data[0]  * m.data[6] * 0.f + 
               m.data[4]  * m.data[1] * 0.f - 
               m.data[4]  * m.data[2] * 0.f - 
               0.f * m.data[1] * m.data[6] + 
               0.f * m.data[2] * m.data[5];

    inv[3] = -m.data[1] * m.data[6] * m.data[11] + 
              m.data[1] * m.data[7] * m.data[10] + 
              m.data[5] * m.data[2] * m.data[11] - 
              m.data[5] * m.data[3] * m.data[10] - 
              m.data[9] * m.data[2] * m.data[7] + 
              m.data[9] * m.data[3] * m.data[6];

    inv[7] = m.data[0] * m.data[6] * m.data[11] - 
             m.data[0] * m.data[7] * m.data[10] - 
             m.data[4] * m.data[2] * m.data[11] + 
             m.data[4] * m.data[3] * m.data[10] + 
             m.data[8] * m.data[2] * m.data[7] - 
             m.data[8] * m.data[3] * m.data[6];

    inv[11] = -m.data[0] * m.data[5] * m.data[11] + 
               m.data[0] * m.data[7] * m.data[9] + 
               m.data[4] * m.data[1] * m.data[11] - 
               m.data[4] * m.data[3] * m.data[9] - 
               m.data[8] * m.data[1] * m.data[7] + 
               m.data[8] * m.data[3] * m.data[5];

    inv[15] = m.data[0] * m.data[5] * m.data[10] - 
              m.data[0] * m.data[6] * m.data[9] - 
              m.data[4] * m.data[1] * m.data[10] + 
              m.data[4] * m.data[2] * m.data[9] + 
              m.data[8] * m.data[1] * m.data[6] - 
              m.data[8] * m.data[2] * m.data[5];

	// Calculate determinant
    det = m.data[0] * inv[0] + m.data[1] * inv[4] + m.data[2] * inv[8] + m.data[3] * inv[12];

	// If the determinant is zero, GTFO.
    assert(det != 0.f);

    det = 1.0 / det;

    for (int i = 0; i < 12; i++)
        data[i] = inv[i] * det;
}

Vect3 Matrix4::operator*(const Vect3& vect) const {
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
	// If you understand the reasoning behind this, please put
	//  a description in this comment, and email the updated file
	//  to me. Somehow. Or not. I don't care. Good for you.
	// This is another thing I just sorta ripped off somebody somewhere.
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

Vect3 Matrix4::transformDirection(const Vect3& vector) const {
	// Multiply only by the 3x3 representation of this matrix.
	//  This scales and rotates, but does not translate.
	return Vect3(
		data[0] * vector.x +
		data[1] * vector.y +
		data[2] * vector.z,

		data[4] * vector.x +
		data[5] * vector.y +
		data[6] * vector.z,

		data[8] * vector.x +
		data[9] * vector.y +
		data[10] * vector.z);
}

Vect3 Matrix4::transformInverseDirection(const Vect3& vector) const {
	// Luckily, A transpose is equal to A inverse for
	//  rotational/scaled(?) matrices, so we can just use this:
	return Vect3(
		data[0] * vector.x +
		data[4] * vector.y +
		data[8] * vector.z,

		data[1] * vector.x +
		data[5] * vector.y +
		data[9] * vector.z,

		data[2] * vector.x +
		data[6] * vector.y +
		data[10] * vector.z);
}

Vect3 Frost::localToWorld(const Vect3 & local, const Matrix4& transform) {
	return transform * local;
}

Vect3 Frost::worldToLocal(const Vect3 & world, const Matrix4& transform) {
	Matrix4 inverseTransform;
	inverseTransform.setInverse(transform);

	return inverseTransform * world;
}

Vect3 Matrix4::transform(const Vect3& vect) const {
	return (*this) * vect;
}

Vect3 Matrix4::transformInverse(const Vect3& vect) const {
	Vect3 tmp = vect;
	tmp.x -= data[3];
	tmp.y -= data[7];
	tmp.z -= data[11];
	return Vect3(
		tmp.x * data[0] +
		tmp.y * data[4] +
		tmp.z * data[8],

		tmp.x * data[1] +
		tmp.y * data[5] +
		tmp.z * data[9],

		tmp.x * data[2] +
		tmp.y * data[6] +
		tmp.z * data[10]
		);
}

Vect3 Frost::localToWorldDirn(const Vect3& local, const Matrix4& transform) {
	return transform.transformDirection(local);
}

Vect3 Frost::worldToLocalDirn(const Vect3& world, const Matrix4& transform) {
	return transform.transformInverseDirection(world);
}