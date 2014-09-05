#ifndef FROSTDLL_API
#ifdef _WINDLL
#define FROSTDLL_API __declspec(dllexport)
#else
#define FROSTDLL_API __declspec(dllimport)
#endif
#endif

/////////////////////////////////////////
// Vect3: A 3-dimensional vector class,
//  supporting standard vector math funcs
/////////////////////////////////////////

#ifndef FROST_MATH_VECT3_H
#define FROST_MATH_VECT3_H

#include <cmath>
#include "FloatStructs.h"
#include "FrostExceptions.h"

namespace Frost
{
	class Vect3 : public FLOAT3
	{
	public:
		/////////////////// CTORS //////////////////////
		
		// Default ctor: initialize to zero vector
		FROSTDLL_API Vect3();
		// Initialize component-wise
		FROSTDLL_API Vect3(float x, float y, float z);
		// Initialize with another Vect3 or other R32B32G32_FLOAT type object
		FROSTDLL_API Vect3(const FLOAT3& right);
		// Initialize with another Vect3 or other R32B32G32_FLOAT type object
		FROSTDLL_API Vect3(const Vect3& right);

		///////////// OPERATOR OVERLOADS /////////////
		FROSTDLL_API Vect3 operator+(const FLOAT3& other) const;
		FROSTDLL_API Vect3 operator-(const FLOAT3& other) const;
		FROSTDLL_API float operator*(const FLOAT3& other) const;
		FROSTDLL_API Vect3 operator%(const FLOAT3& other) const;
		FROSTDLL_API Vect3 operator*(float other) const;
		FROSTDLL_API Vect3 operator/(float other) const;
		FROSTDLL_API Vect3& operator+=(const FLOAT3& other);
		FROSTDLL_API Vect3& operator-=(const FLOAT3& other);
		FROSTDLL_API Vect3& operator*=(float other);
		FROSTDLL_API Vect3& operator/=(float other);
		FROSTDLL_API float operator[](int i) const;
		FROSTDLL_API float& operator[](int i);
		FROSTDLL_API Vect3& operator=(const FLOAT3& other);
		FROSTDLL_API bool operator==(const FLOAT3& other);

		///////////// OTHER VECTOR FUNCTIONS //////////////
		FROSTDLL_API float Magnitude();
		FROSTDLL_API float SquareMagnitude();

	private:
	};

	// Perform a dot product operation on the two specified vectors
	FROSTDLL_API float DotProduct(const Vect3& v1, const Vect3& v2);

	// Perform a cross product operation on the two specified vectors
	FROSTDLL_API Vect3 CrossProduct(const Vect3& v1, const Vect3& v2);

	// Perform a component-wise multiplication on the two specified vectors
	FROSTDLL_API Vect3 ComponentProduct(const Vect3& v1, const Vect3& v2);
}

#endif
