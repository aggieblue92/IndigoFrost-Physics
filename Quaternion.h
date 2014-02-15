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

		Quaternion();
		Quaternion(float _r, float _i, float _j, float _k);

		// Normalize the quaternion
		void normalize();

		// Multiply quaternion by quaternion
		void operator *=(const Quaternion& multiplier);

		void rotateByVector(const Vect3& vector, float scale);

		// Add a scaled vector...
		void addScaledVector(const Vect3& vector, float scale);
	};
}

#endif
