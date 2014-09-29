/////////////////////////////////////////
// Vect3: A 3-dimensional vector class,
//  supporting standard vector math funcs
/////////////////////////////////////////

#ifndef FROST_MATH_VECT3_H
#define FROST_MATH_VECT3_H

#include "FloatStructs.h"
#include "FrostExceptions.h"

namespace Frost
{
	class Vect3 : public FLOAT3
	{
	public:
		/////////////////// CTORS //////////////////////
		
		// Default ctor: initialize to zero vector
		Vect3();
		// Initialize component-wise
		Vect3(float x, float y, float z);
		// Initialize with another Vect3 or other R32B32G32_FLOAT type object
		Vect3(const FLOAT3& right);
		// Initialize with another Vect3 or other R32B32G32_FLOAT type object
		Vect3(const Vect3& right);

		///////////// OPERATOR OVERLOADS /////////////
		Vect3 operator+(const FLOAT3& other) const;
		Vect3 operator-(const FLOAT3& other) const;
		float operator*(const FLOAT3& other) const;
		Vect3 operator%(const FLOAT3& other) const;
		Vect3 operator*(float other) const;
		Vect3 operator/(float other) const;
		Vect3& operator+=(const FLOAT3& other);
		Vect3& operator-=(const FLOAT3& other);
		Vect3& operator*=(float other);
		Vect3& operator/=(float other);
		float operator[](int i) const;
		float& operator[](int i);
		Vect3& operator=(const FLOAT3& other);
		bool operator==(const FLOAT3& other) const;

		///////////// OTHER VECTOR FUNCTIONS //////////////
		float Magnitude() const;
		float SquareMagnitude() const;

	private:
	};

	// Perform a dot product operation on the two specified vectors
	float DotProduct(const Vect3& v1, const Vect3& v2);

	// Perform a cross product operation on the two specified vectors
	Vect3 CrossProduct(const Vect3& v1, const Vect3& v2);

	// Perform a component-wise multiplication on the two specified vectors
	Vect3 ComponentProduct(const Vect3& v1, const Vect3& v2);

	Vect3 operator*(float n, const Vect3& o);
}

#endif
