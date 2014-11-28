#include <Vect3.h>
#include <FrostMath.h>
#include <iostream>
#include <string>
#include "Vect3_test.h"

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

	return TestResult(true, "");
}