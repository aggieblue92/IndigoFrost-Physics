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
		Vect3Normal();
		// Initialize component-wise
		Vect3Normal(float x, float y, float z);
		// Initialize with another Vect3 or other R32B32G32_FLOAT structure
		Vect3Normal(const FLOAT3& right);
		// Initialize with another Vect3
		Vect3Normal(const Vect3& right);
		// Initialize with another normal vector (copy ctor)
		Vect3Normal(const Vect3Normal& right);

		///////////// OPERATOR OVERLOADS /////////////
		Vect3Normal& operator=(const FLOAT3& other);
		Vect3Normal& operator=(const Vect3Normal& other);

	private:
		// Define invalid operations here

		// FORBIDDEN!
		void operator+=(const FLOAT3&) {}
		// FORBIDDEN!
		void operator/=(float) {}
		// FORBIDDEN!
		void operator*=(float) {}
		// FORBIDDEN!
		void operator-=(const FLOAT3&) {}
	};
}

#endif
