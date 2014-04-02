#ifndef _FROST_QUATERNION_H_
#define _FROST_QUATERNION_H_

/*****************************************************\

		Quaternion: quaternion structure

Quaternions handle orientations (rotations) in 3D space
  very well. The math is a bit hairy, but essentially,
  a quaternion is a "vector" with four elements, which
  represents a structure with one real part and three
  imaginary components (much like complex numbers have
  one real part and one imaginary component).

Quaternion q = a + bi + cj + dk

The following properties define how quaternions are
  combined: multiplication is handled FOIL-like
  (remember multiplying polynomials in Algebra I?)

i^2 = j^2 = k^2 = ijk = -1

ij = k, ji = -k,
jk = i, kj = -i,
ki = j, ik = -j

A rotation of theta radians about an axis <x, y, z>
  can be expressed in a quaternion as such:

q = cos(theta / a) + x*sin(theta / 2)*i +
      y*sin(theta / 2)*j + z*sin(theta / 2)*k

To express a two rotations, multiply the quaternions
  together, much like matrix multiplication - that is,
  the order of application goes from right to left.

\*****************************************************/

#include <cmath>
#include "Vect3.h"

namespace Frost {
	class Quaternion {
	public:
		// Union holds data as accessible either
		//  as r, i, j, k elements or as data[0-3] elements
		union {
			struct {
				// r: real component coefficient
				// i: first imaginary component coefficient
				// j: second imaginary comopnent coefficient
				// k: third imaginary component coefficient
				float r, i, j, k;
			};

			float data[4];
		};

		// Default ctor - initialize no rotation
		Quaternion();

		// Initialize with values r, i, j, k manually.
		Quaternion(float _r, float _i, float _j, float _k);

		// Normalize the quaternion
		void normalize();

		// Multiply quaternion by quaternion
		void operator *=(const Quaternion& multiplier);

		// Rotate a quaternion by a vector representing
		//  a rotation amount to add. This is only used
		//  to update a quaternion by a rotation over time usually.
		void rotateByVector(const Vect3& vector, float scale);

		// Add a scaled vector...
		void addScaledVector(const Vect3& vector, float scale);

		// Rotate by a vector...
		void rotateByVector(const Vect3& vect);

		// Set to be a rotation of angle (theta) in radians around an axis (axis)
		void setAxisAngleRotation(const Vect3& axis, float angle);
	};
}

#endif
