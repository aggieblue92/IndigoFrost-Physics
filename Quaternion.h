#ifndef _FROST_QUATERNION_H_
#define _FROST_QUATERNION_H_

#include <cmath>
#include "PhysicsClass.h"

namespace Frost {
	class Quaternion {
	public:
		union {
			struct {
				// Holds real component and complex components, r, i, j, k.
				float r, i, j, k;
			};

			float data[4];
		};

		Quaternion() : r(0.0f), i(0.0f), j(0.0f), k(0.0f) {}
		Quaternion(float _r, float _i, float _j, float _k)
			: r(_r), i(_i), j(_j), k(_k) {}

		// Normalize the quaternion
		void normalize() {
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
	};

		// Multiply quaternion by quaternion
		void operator *=(const Quaternion& multiplier) {
			Quaternion q = *this;
			
			r = q.r*multiplier.r - q.i*multiplier.i - q.j*multiplier.j - q.k*multiplier.k;
			i = q.r*multiplier.i + q.i*multiplier.r + q.j*multiplier.k - q.k*multiplier.j;
			j = q.r*multiplier.j + q.j*multiplier.r + q.k*multiplier.i - q.i*multiplier.k;
			k = q.r*multiplier.k + q.k*multiplier.r + q.i*multiplier.j - q.j*multiplier.i;
		}

		void rotateByVector(const Vect3& vector, float scale) {
			Quaternion q(0.0f, vector.x * scale,
				vector.y * scale, vector.z * scale);
			(*this) *= q;
		}

		// Add a scaled vector...
		void addScaledVector(const Vect3& vector, float scale) {
			Quaternion q(0.0f,
				vector.x * scale,
				vector.y * scale,
				vector.z * scale);
			q *= this;
			r += q.r * 0.5f;
			i += q.i * 0.5f;
			j += q.j * 0.5f;
			k += q.k * 0.5f;
		}
}

#endif
