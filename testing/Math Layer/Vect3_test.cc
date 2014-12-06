#include <Vect3.h>
#include <FrostMath.h>
#include <iostream>
#include <string>
#include "Vect3_test.h"

namespace Vect3Tests
{
	TestResult ConstructorTests()
	{
		// Default constructor
		Frost::Vect3 defaultVector;
		if (defaultVector != Frost::MathConstants::VECTOR_ZERO)
		{
			return TestResult(false, "Default constructor creates non-zero vector");
		}

		// 3-piece constructor
		Frost::Vect3 zeroVector(0.f, 0.f, 0.f), onesVector(1.f, 1.f, 1.f), negativeOnesVector(-1.f, -1.f, -1.f)
			, xOneVect(1.f, 0.f, 0.f), yOneVect(0.f, 1.f, 0.f), zOneVect(0.f, 0.f, 1.f);
		if (zeroVector != Frost::MathConstants::VECTOR_ZERO)
			return TestResult(false, "Manual initialization of zero vector failed");
		if (onesVector != Frost::MathConstants::VECTOR_ONE)
			return TestResult(false, "Manual initialization of ones vector failed");
		if (negativeOnesVector._x != -1.f || negativeOnesVector._y != -1.f || negativeOnesVector._z != -1.f)
			return TestResult(false, "Manual initialization of negative ones vector failed");
		if (xOneVect != Frost::MathConstants::VECTOR_UNIT_X)
			return TestResult(false, "Initialization of unit x vector failed!");
		if (yOneVect != Frost::MathConstants::VECTOR_UNIT_Y)
			return TestResult(false, "Initialization of unit y vector failed!");
		if (zOneVect != Frost::MathConstants::VECTOR_UNIT_Z)
			return TestResult(false, "Initialization of unit z vector failed!");

		// Copy Constructor
		Frost::Vect3 simpleTestVector(1.f, 2.f, 3.f);
		Frost::Vect3 simpleCopyVector(simpleTestVector);
		if (simpleTestVector != simpleCopyVector)
			return TestResult(false, "Copy constructor failed on <1, 2, 3>");

		Frost::Vect3 simpleCopyZeroVector(Frost::MathConstants::VECTOR_ZERO);
		if (simpleCopyZeroVector != Frost::MathConstants::VECTOR_ZERO)
			return TestResult(false, "Copy constructor failed for VECTOR_ZERO");

		// Copy from Other Data Types
		Frost::Vect3Normal vn(1.f, 0.f, 0.f);
		Frost::Vect3 xnc(vn);
		if (xnc != Frost::MathConstants::VECTOR_UNIT_X)
			return TestResult(false, "Copy constructor failed from Vect3Normal <1, 0, 0>");

		Frost::FLOAT3 f3;
		f3._x = 1.f; f3._y = 2.f; f3._z = 3.4f;
		Frost::Vect3 v3(f3);
		if (v3._x != 1.f || v3._y != 2.f || v3._z != 3.4f)
			return TestResult(false, "Copy constructor failed for FLOAT3 <1, 2, 3.4>");

		Frost::Vect3 v3initlist({ 3.3f, 3.3f, 3.3f });
		if (v3initlist != Frost::Vect3(3.3f, 3.3f, 3.3f))
			return TestResult(false, "Copy constructor failed from initializer list <3.3, 3.3, 3.3>");

		// Constant Vector
		const Frost::Vect3 v3c(1.f, 2.f, 3.f);
		if (v3c._x != 1.f || v3c._y != 2.f || v3c._z != 3.f)
			return TestResult(false, "Regular construction of a constant vector failed");

		const Frost::Vect3 v3ci({ 1.f, 2.f, 3.f });
		if (v3c != v3ci)
			return TestResult(false, "Initializer list construction of a constant vector failed");

		const Frost::Vect3 v3cf3(f3);
		if (v3cf3._x != 1.f || v3cf3._y != 2.f || v3cf3._z != 3.4f)
			return TestResult(false, "FLOAT3 copy constructor of constant Vect3 failed");

		return TEST_PASSED;
	}

	TestResult VectorArithmeticTests()
	{
		// Basic Addition Tests
		// Basic Subtraction Tests
		// Addition Assignment Tests
		// Subtraction Assignment Tests
		// Edge Cases
		// Against Constants

		return TEST_PASSED;
	}

	TestResult ScalarTests()
	{
		// Scalar Multiplication
		// Scalar Division
		// Against Constants
		// Divide by Zero
		return TEST_PASSED;
	}

	TestResult AccessorMethodTests()
	{
		// Square brackets access, access and modify
		return TEST_PASSED;
	}

	TestResult VectorFunctionTests()
	{
		// Scalar Product
		// Vector Product
		// Magnitude, Square Magnitude
		// External scalar, vector, component product
		return TEST_PASSED;
	}

	TestResult ApproximationTests()
	{
		// Equality Tests
		Frost::Vect3 zero_vector(0.f, 0.f, 0.f);
		if (!(zero_vector == Frost::MathConstants::VECTOR_ZERO))
			return TestResult(false, "Failed to equate zero vectors");
		if (zero_vector != Frost::MathConstants::VECTOR_ZERO)
			return TestResult(false, "Failed to reject inequality of zero vectors");

		Frost::Vect3 ones_vector(1.f, 1.f, 1.f);
		if (!(ones_vector == Frost::MathConstants::VECTOR_ONE))
			return TestResult(false, "Failed to equate ones vectors");
		if (ones_vector != Frost::MathConstants::VECTOR_ONE)
			return TestResult(false, "Failed to reject inequality of ones vectors");

		Frost::Vect3 unit_z(0.f, 0.f, 1.f);
		if (!(unit_z == Frost::MathConstants::VECTOR_UNIT_Z))
			return TestResult(false, "Failed to equate unit z vectors");
		if (unit_z != Frost::MathConstants::VECTOR_UNIT_Z)
			return TestResult(false, "Failed to reject inequality of unit z vectors");

		Frost::Vect3 unit_y(0.f, 1.f, 0.f);
		if (!(unit_y == Frost::MathConstants::VECTOR_UNIT_Y))
			return TestResult(false, "Failed to equate unit y vectors");
		if (unit_y != Frost::MathConstants::VECTOR_UNIT_Y)
			return TestResult(false, "Failed to reject inequality of unit y vectors");

		Frost::Vect3 unit_x(1.f, 0.f, 0.f);
		if (!(unit_x == Frost::MathConstants::VECTOR_UNIT_X))
			return TestResult(false, "Failed to equate unit z vectors");
		if (unit_x != Frost::MathConstants::VECTOR_UNIT_X)
			return TestResult(false, "Failed to reject inequality of unit x vectors");

		if (unit_x == unit_y)
			return TestResult(false, "Equated unit x and unit y vectors");
		if (unit_x == unit_z)
			return TestResult(false, "Equated unit x and unit z vectors");
		if (ones_vector == unit_x)
			return TestResult(false, "Equated ones vector with unit x vector");
		if (zero_vector == ones_vector)
			return TestResult(false, "Equated ones vector with zero vector");

		// isApproximately functions
		// Negative Numbers
		// Zero
		// Excpected Case
		return TEST_PASSED;
	}

}