#include "Quaternion.h"
using namespace Frost;

Quaternion::Quaternion()
	: r(0.0f), i(0.0f), j(0.0f), k(0.0f)
{}

Quaternion::Quaternion(float _r, float _i, float _j, float _k)
	: r(_r), i(_i), j(_j), k(_k)
{}

void Quaternion::normalize(){
	float d = r*r + i*i + j*j + k*k;

	// If zero length, eh, use no-rotation.
	if(0 == d) {
		r = 1;
		return;
	}

	d = (1.0f / std::sqrt(d));
	r *= d;
	i *= d;
	j *= d;
	k *= d;
}

void Quaternion::operator*=(const Quaternion& multiplier) {
	Quaternion q = *this;

	r = q.r*multiplier.r - q.i*multiplier.i - q.j*multiplier.j - q.k*multiplier.k;
	i = q.r*multiplier.i + q.i*multiplier.r + q.j*multiplier.k - q.k*multiplier.j;
	j = q.r*multiplier.j + q.j*multiplier.r + q.k*multiplier.i - q.i*multiplier.k;
	k = q.r*multiplier.k + q.k*multiplier.r + q.i*multiplier.j - q.j*multiplier.i;
}

void Quaternion::rotateByVector(const Vect3& vector, float scale) {
	Quaternion q(0.0f, vector.x * scale,
		vector.y * scale, vector.z * scale);
	(*this) *= q;
}

void Quaternion::addScaledVector(const Vect3& vector, float scale) {
	Quaternion q(0.0f,
		vector.x * scale,
		vector.y * scale,
		vector.z * scale);
	q *= *this;
	r += q.r * 0.5f;
	i += q.i * 0.5f;
	j += q.j * 0.5f;
	k += q.k * 0.5f;
}