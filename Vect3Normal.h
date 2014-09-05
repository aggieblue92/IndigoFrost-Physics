#ifndef FROSTDLL_API
#ifdef _WINDLL
#define FROSTDLL_API __declspec(dllexport)
#else
#define FROSTDLL_API __declspec(dllimport)
#endif
#endif

/////////////////////////////////////////
// Vect3Normal: Implementation of vector
//  class that enforces normality on read.
/////////////////////////////////////////

#ifndef FROST_MATH_VECT3_NORMAL_H
#define FROST_MATH_VECT3_NORMAL_H

#include "Vect3.h"

namespace Frost
{
	// Exceptions...
	class Vect3Normal : public Vect3
	{
	public:
		/////////////////// CTORS //////////////////////
		// Default ctor - initialize to the unit X vector
		FROSTDLL_API Vect3Normal();
		// Initialize component-wise
		FROSTDLL_API Vect3Normal(float x, float y, float z);
		// Initialize with another Vect3 or other R32B32G32_FLOAT structure
		FROSTDLL_API Vect3Normal(const FLOAT3& right);
		// Initialize with another Vect3
		FROSTDLL_API Vect3Normal(const Vect3& right);
		// Initialize with another normal vector (copy ctor)
		FROSTDLL_API Vect3Normal(const Vect3Normal& right);

		///////////// OPERATOR OVERLOADS /////////////
		FROSTDLL_API Vect3Normal& operator=(const FLOAT3& other);
		FROSTDLL_API Vect3Normal& operator=(const Vect3Normal& other);

	private:
		// Define invalid operations here

		// FORBIDDEN!
		FROSTDLL_API void operator+=(const FLOAT3&) {}
		// FORBIDDEN!
		FROSTDLL_API void operator/=(float) {}
		// FORBIDDEN!
		FROSTDLL_API void operator*=(float) {}
		// FORBIDDEN!
		FROSTDLL_API void operator-=(const FLOAT3&) {}
	};
}

#endif
