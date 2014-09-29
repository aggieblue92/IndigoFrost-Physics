/////////////////////////////////////////
// Matrix - 4x4 matrix, used for transformations
/////////////////////////////////////////

#ifndef FROST_MATRIX_CLASS_H
#define FROST_MATRIX_CLASS_H

#include "FrostExceptions.h"
#include "FloatStructs.h"
#include "Vect3Normal.h"
#include "Quaternion.h"

namespace Frost
{
	class Matrix : public FLOAT4X4
	{
	public:
		/////////////////// CTORS //////////////////////

		// Default ctor - identity matrix
		Matrix();
		// Copy ctor
		Matrix(const Matrix& other);
		// Initialize with any other R32B32G32A32_FLOAT[4] structure
		Matrix(const FLOAT4X4& other);
		// Initialize with four columns
		Matrix(const FLOAT4& col1, const FLOAT4& col2, const FLOAT4& col3, const FLOAT4& col4);
		// Initialize with individual values
		Matrix(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44);
		// From a rotation (i.e., make a rotation matrix)
		Matrix(const Quaternion& quat);
		// From a translation (i.e., make a translation matrix)
		Matrix(const Vect3& translation);
		// From a rotation and translation
		Matrix(const Quaternion& rotation, const FLOAT3& translation);

		///////////// OPERATOR OVERLOADS /////////////
		Matrix& operator=(const FLOAT4X4& other);
		Matrix operator+(const FLOAT4X4& other) const;
		Matrix operator-(const FLOAT4X4& other) const;
		Matrix operator*(const FLOAT4X4& other) const;
		FLOAT3 operator*(const FLOAT3& other) const;
		Matrix operator*(float other) const;
		FLOAT4 operator[](int i) const;
		FLOAT4& operator[](int i);
		bool operator==(const FLOAT4X4& other) const;

		//////////////// OTHER MATRIX FUNCTIONS ////////////////

		// Returns the inverse of the given matrix
		Matrix GetInverse() const;
		Matrix GetTranspose() const;
		void getOrientationAndPosition(Vect3& o_pos, Quaternion& o_orientation) const;
		Quaternion getOrientation() const;
		Vect3 getPosition() const;

		/////////////////// TRANSFORMATION FUNCTIONS /////////////

		// Transform a coordinate - includes scaling, rotation, translation
		FLOAT3 TransformCoord(const FLOAT3& coord) const;
		// Transform a direction - includes scaling, rotation
		FLOAT3 TransformDirn(const FLOAT3& dirn) const;
		// Transform a unit direction
		Vect3Normal TransformNormal(const FLOAT3& dirn) const;
	};
}

#endif