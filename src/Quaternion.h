#ifndef FROSTDLL_API
#ifdef _WINDLL
#define FROSTDLL_API __declspec(dllexport)
#else
#define FROSTDLL_API __declspec(dllimport)
#endif
#endif

/////////////////////////////////////////
// Quaternion: A 4-d vector class, with
//  all the additional bells and whistles
//  of quaternion math
/////////////////////////////////////////

#ifndef FROST_QUATERNION_H
#define FROST_QUATERNION_H

#include "FloatStructs.h"
#include "FrostExceptions.h"
#include "Vect3Normal.h"
#include <cmath>

namespace Frost
{
	class Quaternion : public FLOAT4
	{
	public:
		/////////////////// CTORS //////////////////////

		// Default ctor - initialize with no rotation
		FROSTDLL_API Quaternion();

		// Initialize values manually
		FROSTDLL_API Quaternion(float r, float i, float j, float k);

		// Initialize from another R32G32B32A32_FLOAT structure
		FROSTDLL_API Quaternion(const FLOAT4& other);

		// Initialize from an axis and an angle
		FROSTDLL_API Quaternion(const FLOAT3& axis, float angle);

		///////////// OPERATOR OVERLOADS /////////////
		FROSTDLL_API Quaternion& operator=(const FLOAT4& other);
		FROSTDLL_API Quaternion& operator=(const Quaternion& other);
		FROSTDLL_API Quaternion operator*(const FLOAT4& other) const;
		FROSTDLL_API Quaternion& operator*=(const FLOAT4& other);
		FROSTDLL_API Quaternion operator+(const FLOAT4& o) const
		{
			return Quaternion(
				this->_w + o._w,
				this->_x + o._x,
				this->_y + o._y,
				this->_z + o._z
				);
		}
		FROSTDLL_API Quaternion& operator+=(const FLOAT3& other);
		FROSTDLL_API Quaternion operator+(const FLOAT3& other) const;

		////////////////// QUATERNION FUNCS //////////////////
		FROSTDLL_API void GetAxisAngle(FLOAT3& o_Axis, float& o_Angle) const;
	private:
		float Magnitude();
	};
}

#endif