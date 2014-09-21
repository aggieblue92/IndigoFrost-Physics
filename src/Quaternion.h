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
		Quaternion();

		// Initialize values manually
		Quaternion(float r, float i, float j, float k);

		// Initialize from another R32G32B32A32_FLOAT structure
		Quaternion(const FLOAT4& other);

		// Initialize from an axis and an angle
		Quaternion(const FLOAT3& axis, float angle);

		///////////// OPERATOR OVERLOADS /////////////
		Quaternion& operator=(const FLOAT4& other);
		Quaternion& operator=(const Quaternion& other);
		Quaternion operator*(const FLOAT4& other) const;
		Quaternion& operator*=(const FLOAT4& other);
		Quaternion operator+(const FLOAT4& o) const
		{
			return Quaternion(
				this->_w + o._w,
				this->_x + o._x,
				this->_y + o._y,
				this->_z + o._z
				);
		}
		Quaternion& operator+=(const FLOAT3& other);
		Quaternion operator+(const FLOAT3& other) const;

		////////////////// QUATERNION FUNCS //////////////////
		void GetAxisAngle(FLOAT3& o_Axis, float& o_Angle) const;
	private:
		float Magnitude();
	};
}

#endif