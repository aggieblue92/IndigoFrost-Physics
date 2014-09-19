#ifndef FROSTDLL_API
#ifdef _WINDLL
#define FROSTDLL_API __declspec(dllexport)
#else
#define FROSTDLL_API __declspec(dllimport)
#endif
#endif

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
		FROSTDLL_API Matrix();
		// Copy ctor
		FROSTDLL_API Matrix(const Matrix& other);
		// Initialize with any other R32B32G32A32_FLOAT[4] structure
		FROSTDLL_API Matrix(const FLOAT4X4& other);
		// Initialize with four columns
		FROSTDLL_API Matrix(const FLOAT4& col1, const FLOAT4& col2, const FLOAT4& col3, const FLOAT4& col4);
		// Initialize with individual values
		FROSTDLL_API Matrix(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44);
		// From a rotation (i.e., make a rotation matrix)
		FROSTDLL_API Matrix(const Quaternion& quat);
		// From a translation (i.e., make a translation matrix)
		FROSTDLL_API Matrix(const Vect3& translation);
		// From a rotation and translation
		FROSTDLL_API Matrix(const Quaternion& rotation, const FLOAT3& translation);

		///////////// OPERATOR OVERLOADS /////////////
		FROSTDLL_API Matrix& operator=(const FLOAT4X4& other);
		FROSTDLL_API Matrix operator+(const FLOAT4X4& other) const;
		FROSTDLL_API Matrix operator-(const FLOAT4X4& other) const;
		FROSTDLL_API Matrix operator*(const FLOAT4X4& other) const;
		FROSTDLL_API FLOAT3 operator*(const FLOAT3& other) const;
		FROSTDLL_API Matrix operator*(float other) const;
		FROSTDLL_API FLOAT4 operator[](int i) const;
		FROSTDLL_API FLOAT4& operator[](int i);
		FROSTDLL_API bool operator==(const FLOAT4X4& other) const;

		//////////////// OTHER MATRIX FUNCTIONS ////////////////

		// Returns the inverse of the given matrix
		FROSTDLL_API Matrix GetInverse() const;
		FROSTDLL_API Matrix GetTranspose() const;
		/*FROSTDLL_API void GetRotationAndTranslation(Quaternion& o_Rotation, FLOAT3& o_Translation);
		FROSTDLL_API void GetRotation(Quaternion& o_Rotation);
		FROSTDLL_API void GetTranslation(FLOAT3& o_Translation);*/
		// FROSTDLL_API Matrix GetPerspectiveFovLH(AspectRatio, Whatever);
		// FROSTDLL_API Matrix GetViewMatrix(LookAt, Up, Whatever);

		/////////////////// TRANSFORMATION FUNCTIONS /////////////

		// Transform a coordinate - includes scaling, rotation, translation
		FROSTDLL_API FLOAT3 TransformCoord(const FLOAT3& coord) const;
		// Transform a direction - includes scaling, rotation
		FROSTDLL_API FLOAT3 TransformDirn(const FLOAT3& dirn) const;
		// Transform a unit direction
		FROSTDLL_API Vect3Normal TransformNormal(const FLOAT3& dirn) const;
	};
}

#endif