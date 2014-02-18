#ifndef INDIGO_FROST_PHYSICS_CLASS_H
#define INDIGO_FROST_PHYSICS_CLASS_H

/*****************************************************\

		Vect3: A 3-dimensional vector class,
	with standard vector functions supported.

\*****************************************************/

#include <assert.h>
#include <math.h>

namespace Frost {
	struct Vect3 {
		float x, y, z;

		// Default ctor: initialize to zero vector
		Vect3() : x(0.0f), y(0.0f), z(0.0f) {}

		// Initialize vector 
		Vect3(float xPos, float yPos, float zPos) :
			x(xPos), y(yPos), z(zPos) {}
		Vect3 operator+(const Vect3 right) {
			return Vect3(x + right.x, y + right.y, z + right.z);
		}
		Vect3 operator-(const Vect3 right) {
			return Vect3(x - right.x, y - right.y, z - right.z);
		}
		Vect3& operator+=(const Vect3 right) {
			return Vect3(x += right.x, y += right.y, z += right.z);
		}
		Vect3& operator-=(const Vect3 right) {
			return Vect3(x -= right.x, y -= right.y, z -= right.z);
		}
		float operator*(const Vect3 right) const {
			return (x * right.x + y * right.y + z * right.z);
		}
		Vect3& operator*(const float right) {
			return Vect3(x * right, y * right, z * right);
		}
		Vect3 operator*=(const float right) {
			return Vect3(x *= right, y *= right, z *= right);
		}
		static Vect3 CrossProduct(const Vect3 left, const Vect3 right) {
			return Vect3(left.y * right.z - left.z * right.y,
				left.z * right.x - left.x * right.z,
				left.x * right.y - left.y * right.x);
		}
		void Normalize() {
			float divBy = sqrt((x * x) + (y * y) + (z * z));
			// If divBy is zero, then x,y,z are all zero.
			if (0 == divBy)
				divBy = 1.0f;
			x /= divBy;
			y /= divBy;
			z /= divBy;
		}
		float Magnitude() {
			return sqrt((x * x) + (y * y) + (z * z));
		}
		Vect3 GetNormal() {
			Vect3 toReturn(*this);
			toReturn.Normalize();
			return toReturn;
		}
	};
}

#endif
