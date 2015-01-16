#include "stdafx.h"
#include "CppUnitTest.h"
#include <Vect3.h>
#include <FrostMath.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<> static std::wstring ToString<Frost::Vect3>(const Frost::Vect3& v)
			{
				std::wstringstream wss(L"");
				wss << L"[" << v._x << L", " << v._y << ", " << v._z << "]";
				return wss.str();
			}
		}
	}
}

namespace UnitTests
{
	TEST_CLASS(Vect3_test)
	{
	public:
		
		TEST_METHOD(ctor)
		{
			// Default Constructor
			Frost::Vect3 defaultVector;
			Assert::AreEqual(defaultVector, Frost::MathConstants::VECTOR_ZERO);

			// 3-piece Constructor
			Frost::Vect3 zeroVector(0.f, 0.f, 0.f), onesVector(1.f, 1.f, 1.f), negativeOnesVector(-1.f, -1.f, -1.f)
				, xOneVect(1.f, 0.f, 0.f), yOneVect(0.f, 1.f, 0.f), zOneVect(0.f, 0.f, 1.f);
			Assert::AreEqual(zeroVector, Frost::MathConstants::VECTOR_ZERO, L"Manual initialization of zero vector falied");
			Assert::AreEqual(onesVector, Frost::MathConstants::VECTOR_ONE, L"Manual initialization of ones vector falied");
			Assert::AreEqual(negativeOnesVector, Frost::MathConstants::VECTOR_ONE * -1.f, L"Manual initialization of negative ones vector failed");
			Assert::AreEqual(xOneVect, Frost::MathConstants::VECTOR_UNIT_X, L"Initialization of unit x vector failed");
			Assert::AreEqual(yOneVect, Frost::MathConstants::VECTOR_UNIT_Y, L"Initialization of unit y vector falied");
			Assert::AreEqual(zOneVect, Frost::MathConstants::VECTOR_UNIT_Z, L"Initialization of unit z vector failed");

			// Assignment Operator
			Frost::Vect3 foo;
			foo = Frost::MathConstants::VECTOR_ONE;
			Assert::AreEqual(Frost::MathConstants::VECTOR_ONE, foo, L"Assignment operator initialization failed for one vector");

			float bar[3] = { 1.f, 3.f, 6.f };
			foo = *reinterpret_cast<Frost::FLOAT3*>(bar);
			Assert::AreEqual(Frost::Vect3(1.f, 3.f, 6.f), foo, L"Assignment operator from foreign FLOAT3 struct initialization failed");

			// Copy Constructor
			Frost::Vect3 simpleTestVector(1.f, 2.f, 3.f);
			Frost::Vect3 simpleCopyVector(simpleTestVector);
			Assert::AreEqual(simpleTestVector, simpleCopyVector, L"Copy constructor failed");
			Assert::AreNotSame(simpleTestVector, simpleCopyVector, L"Copy constructor copied reference");

			Frost::Vect3 simpleCopyOneVector(Frost::MathConstants::VECTOR_ONE);
			Assert::AreEqual(simpleCopyOneVector, Frost::MathConstants::VECTOR_ONE, L"Copy constructor failed for constant ones vector");

			// Copy from other data types
			Frost::Vect3Normal vn(1.f, 0.f, 0.f);
			Frost::Vect3 xnc(vn);
			Assert::AreEqual(xnc, Frost::MathConstants::VECTOR_UNIT_X, L"Failed to copy Vect3Normal to Vect3");

			Frost::FLOAT3 f3; f3._x = 1.f; f3._y = 2.f; f3._z = 3.4f;
			Frost::Vect3 v3(f3);
			Assert::AreEqual(v3._x, 1.f, L"FLOAT3 -> Vect3 x did not correctly copy");
			Assert::AreEqual(v3._y, 2.f, L"FLOAT3 -> Vect3 y did not correctly copy");
			Assert::AreEqual(v3._z, 3.4f, L"FLOAT3 -> Vect3 z did not correctly copy");

			Frost::Vect3 v3initlist({ 3.3f, 3.4f, 3.5f });
			Assert::AreEqual(v3initlist, Frost::Vect3(3.3f, 3.4f, 3.5f), L"Copy constructor failed from initialization list");

			// Constant Vector
			const Frost::Vect3 v3c(1.f, 2.f, 3.f);
			Assert::AreEqual(v3c, Frost::Vect3(1.f, 2.f, 3.f), L"Regular construction of a constant vector failed");

			const Frost::Vect3 v3ci({ 1.f, 2.f, 3.f });
			Assert::AreEqual(v3ci, v3c, L"Initializer list construction of constant vector failed");

			const Frost::Vect3 v3cf3(f3);
			Assert::AreEqual(v3cf3, Frost::Vect3(f3._x, f3._y, f3._z), L"FLOAT3 copy constructor of constant Vect3 failed");
		}

		TEST_METHOD(constantDefinitions)
		{
			Assert::AreEqual(Frost::MathConstants::VECTOR_ZERO, Frost::Vect3(0.f, 0.f, 0.f), L"Zero vector has unexpected value");
			Assert::AreEqual(Frost::MathConstants::VECTOR_ONE, Frost::Vect3(1.f, 1.f, 1.f), L"Ones vector has unexpected value");
			Assert::AreEqual(Frost::MathConstants::VECTOR_UNIT_X, Frost::Vect3(1.f, 0.f, 0.f), L"Unit X vector unexpected value");
			Assert::AreEqual(Frost::MathConstants::VECTOR_UNIT_Y, Frost::Vect3(0.f, 1.f, 0.f), L"Unit Y vector unexpected value");
			Assert::AreEqual(Frost::MathConstants::VECTOR_UNIT_Z, Frost::Vect3(0.f, 0.f, 1.f), L"Unit Z vector unexpected value");

			Assert::AreNotEqual(Frost::MathConstants::VECTOR_UNIT_X, Frost::MathConstants::VECTOR_UNIT_Y);
			Assert::AreNotEqual(Frost::MathConstants::VECTOR_UNIT_X, Frost::MathConstants::VECTOR_UNIT_Z);
			Assert::AreNotEqual(Frost::MathConstants::VECTOR_UNIT_Y, Frost::MathConstants::VECTOR_UNIT_Z);
			Assert::AreNotEqual(Frost::MathConstants::VECTOR_ONE, Frost::MathConstants::VECTOR_ZERO);
			Assert::AreNotEqual(Frost::MathConstants::VECTOR_ONE, Frost::MathConstants::VECTOR_UNIT_X);
			Assert::AreNotEqual(Frost::MathConstants::VECTOR_ONE, Frost::MathConstants::VECTOR_UNIT_Y);
			Assert::AreNotEqual(Frost::MathConstants::VECTOR_ONE, Frost::MathConstants::VECTOR_UNIT_Z);
			Assert::AreNotEqual(Frost::MathConstants::VECTOR_ZERO, Frost::MathConstants::VECTOR_UNIT_X);
			Assert::AreNotEqual(Frost::MathConstants::VECTOR_ZERO, Frost::MathConstants::VECTOR_UNIT_Y);
			Assert::AreNotEqual(Frost::MathConstants::VECTOR_ZERO, Frost::MathConstants::VECTOR_UNIT_Z);
		}

		TEST_METHOD(vectorArithmetic)
		{
			// Basic Addition Tests
			Frost::Vect3 pos1(2.f, 3.f, 5.f), pos2(1.f, 5.f, 3.3f);
			Frost::Vect3 neg1(-4.f, -1.f, -7.f), neg2(-11.f, -2.2f, -5.f);
			Frost::Vect3 mix1(1.f, -2.f, 3.f), mix2(-1.1f, 2.1f, -3.1f);
			Assert::AreEqual(pos1 + pos2, Frost::Vect3(pos1._x + pos2._x, pos1._y + pos2._y, pos1._z + pos2._z), L"Vector addition for two small positive vectors failed");
			Assert::AreEqual(pos1 + neg1, Frost::Vect3(pos1._x + neg1._x, pos1._y + neg1._y, pos1._z + neg1._z), L"Vector addition for small positive, negative vectors failed");
			Assert::AreEqual(neg1 + neg2, Frost::Vect3(neg1._x + neg2._x, neg1._y + neg2._y, neg1._z + neg2._z), L"Vector addition for two small negative vectors failed");
			Assert::AreEqual(pos1 + mix1, Frost::Vect3(pos1._x + mix1._x, pos1._y + mix1._y, pos1._z + mix1._z), L"Vector addition for small positive, mixed vectors failed");
			Assert::AreEqual(neg1 + mix1, Frost::Vect3(neg1._x + mix1._x, neg1._y + mix1._y, neg1._z + mix1._z), L"Vector addition for small negative, mixed vectors failed");
			Assert::AreEqual(mix1 + mix2, Frost::Vect3(mix1._x + mix2._x, mix1._y + mix2._y, mix1._z + mix2._z), L"Vector addition for two small mixed vectors failed");

			// Basic Subtraction Tests
			Assert::AreEqual(pos1 - pos2, Frost::Vect3(pos1._x - pos2._x, pos1._y - pos2._y, pos1._z - pos2._z), L"Vector subtraction for two small positive vectors failed");
			Assert::AreEqual(pos1 - neg1, Frost::Vect3(pos1._x - neg1._x, pos1._y - neg1._y, pos1._z - neg1._z), L"Vector subtraction for small positive, negative vectors failed");
			Assert::AreEqual(neg1 - neg2, Frost::Vect3(neg1._x - neg2._x, neg1._y - neg2._y, neg1._z - neg2._z), L"Vector subtraction for two small negative vectors failed");
			Assert::AreEqual(pos1 - mix1, Frost::Vect3(pos1._x - mix1._x, pos1._y - mix1._y, pos1._z - mix1._z), L"Vector subtraction for small positive, mixed vectors failed");
			Assert::AreEqual(neg1 - mix1, Frost::Vect3(neg1._x - mix1._x, neg1._y - mix1._y, neg1._z - mix1._z), L"Vector subtraction for small negative, mixed vectors failed");
			Assert::AreEqual(mix1 - mix2, Frost::Vect3(mix1._x - mix2._x, mix1._y - mix2._y, mix1._z - mix2._z), L"Vector subtraction for two small mixed vectors failed");

			// Addition Assignment Tests
			Frost::Vect3 pos3 = pos1;
			pos3 += pos2;
			Assert::AreEqual(pos1 + pos2, pos3, L"Addition assignment for positive vector failed");
			Frost::Vect3 neg3 = neg1;
			neg3 += neg2;
			Assert::AreEqual(neg1 + neg2, neg3, L"Addition assignment for negative vector failed");
			Frost::Vect3 mix3 = mix1;
			mix3 += mix2;
			Assert::AreEqual(mix1 + mix2, mix3, L"Addition assignment for mixed vector failed");

			// Subtraction Assignment Tests
			pos3 = pos1;
			pos3 -= pos2;
			Assert::AreEqual(pos1 - pos2, pos3, L"Addition assignment for positive vector failed");
			neg3 = neg1;
			neg3 -= neg2;
			Assert::AreEqual(neg1 - neg2, neg3, L"Addition assignment for negative vector failed");
			mix3 = mix1;
			mix3 -= mix2;
			Assert::AreEqual(mix1 - mix2, mix3, L"Addition assignment for mixed vector failed");

			// Edge Cases and Constants
			Assert::AreEqual(Frost::MathConstants::VECTOR_ZERO + Frost::MathConstants::VECTOR_ZERO, Frost::MathConstants::VECTOR_ZERO, L"Addition corrupted value on zero vector");
			Assert::AreEqual(Frost::MathConstants::VECTOR_ONE - Frost::MathConstants::VECTOR_UNIT_X - Frost::MathConstants::VECTOR_UNIT_Y - Frost::MathConstants::VECTOR_UNIT_Z,
				Frost::MathConstants::VECTOR_ZERO, L"Subtraction of unit vectors from ones vector yielded unexpected value");
			Assert::AreEqual(pos1 - pos1, Frost::MathConstants::VECTOR_ZERO, L"Subtraction of positive vector from itself does not yield zero vector");
			Assert::AreEqual(neg1 - neg1, Frost::MathConstants::VECTOR_ZERO, L"Subtraction of negative vector from itself does not yield zero vector");
			Assert::AreEqual(mix1 - mix1, Frost::MathConstants::VECTOR_ZERO, L"Subtraction of mixed vector from itself does not yield zero vector");
		}

		TEST_METHOD(scalarArithmetic)
		{
			// Scalar Multiplication
			Frost::Vect3 pos1(2.f, 3.f, 5.f), pos2(1.f, 5.f, 3.3f);
			Frost::Vect3 neg1(-4.f, -1.f, -7.f), neg2(-11.f, -2.2f, -5.f);
			Frost::Vect3 mix1(1.f, -2.f, 3.f), mix2(-1.1f, 2.1f, -3.1f);
			Assert::AreEqual(pos1 * 5.f, Frost::Vect3(pos1._x * 5.f, pos1._y * 5.f, pos1._z * 5.f), L"Scalar multiplication of positive vector by positive scalar yields unexpected result");
			Assert::AreEqual(neg1 * 5.f, Frost::Vect3(neg1._x * 5.f, neg1._y * 5.f, neg1._z * 5.f), L"Scalar multiplication of negative vector by positive scalar yields unexpected result");
			Assert::AreEqual(mix1 * 5.f, Frost::Vect3(mix1._x * 5.f, mix1._y * 5.f, mix1._z * 5.f), L"Scalar multiplication of mixed vector by positive scalar yields unexpected result");
			Assert::AreEqual(pos1 * -5.f, Frost::Vect3(pos1._x * -5.f, pos1._y * -5.f, pos1._z * -5.f), L"Scalar multiplication of positive vector by negative scalar yields unexpected result");
			Assert::AreEqual(neg1 * -5.f, Frost::Vect3(neg1._x * -5.f, neg1._y * -5.f, neg1._z * -5.f), L"Scalar multiplication of negative vector by negative scalar yields unexpected result");
			Assert::AreEqual(mix1 * -5.f, Frost::Vect3(mix1._x * -5.f, mix1._y * -5.f, mix1._z * -5.f), L"Scalar multiplication of mixed vector by negative scalar yields unexpected result");

			// Scalar Division
			Assert::AreEqual(pos1 / 5.f, Frost::Vect3(pos1._x / 5.f, pos1._y / 5.f, pos1._z / 5.f), L"Scalar division of positive vector by positive scalar yields unexpected result");
			Assert::AreEqual(neg1 / 5.f, Frost::Vect3(neg1._x / 5.f, neg1._y / 5.f, neg1._z / 5.f), L"Scalar division of negative vector by positive scalar yields unexpected result");
			Assert::AreEqual(mix1 / 5.f, Frost::Vect3(mix1._x / 5.f, mix1._y / 5.f, mix1._z / 5.f), L"Scalar division of mixed vector by positive scalar yields unexpected result");
			Assert::AreEqual(pos1 / -5.f, Frost::Vect3(pos1._x / -5.f, pos1._y / -5.f, pos1._z / -5.f), L"Scalar division of positive vector by negative scalar yields unexpected result");
			Assert::AreEqual(neg1 / -5.f, Frost::Vect3(neg1._x / -5.f, neg1._y / -5.f, neg1._z / -5.f), L"Scalar division of negative vector by negative scalar yields unexpected result");
			Assert::AreEqual(mix1 / -5.f, Frost::Vect3(mix1._x / -5.f, mix1._y / -5.f, mix1._z / -5.f), L"Scalar division of mixed vector by negative scalar yields unexpected result");

			// Multiplication assignment
			Frost::Vect3 pos3 = pos1;
			Frost::Vect3 neg3 = neg1;
			Frost::Vect3 mix3 = mix1;
			pos3 *= 2.f;
			neg3 *= 2.f;
			mix3 *= 2.f;
			Assert::AreEqual(pos1 * 2.f, pos3, L"Multiplication assignment failed for positive vector, positive scalar");
			Assert::AreEqual(neg1 * 2.f, neg3, L"Multiplication assignment failed for negative vector, positive scalar");
			Assert::AreEqual(mix1 * 2.f, mix3, L"Multiplication assignment failed for mixed vector, positive scalar");

			pos3 = pos1;
			neg3 = neg1;
			mix3 = mix1;
			pos3 *= -2.f;
			neg3 *= -2.f;
			mix3 *= -2.f;
			Assert::AreEqual(pos1 * -2.f, pos3, L"Multiplication assignment failed for positive vector, negative scalar");
			Assert::AreEqual(neg1 * -2.f, neg3, L"Multiplication assignment failed for negative vector, negative scalar");
			Assert::AreEqual(mix1 * -2.f, mix3, L"Multiplication assignment failed for mixed vector, negative scalar");

			// Division assignment
			pos3 = pos1;
			neg3 = neg1;
			mix3 = mix1;
			pos3 /= 2.f;
			neg3 /= 2.f;
			mix3 /= 2.f;
			Assert::AreEqual(pos1 / 2.f, pos3, L"Multiplication assignment failed for positive vector, positive scalar");
			Assert::AreEqual(neg1 / 2.f, neg3, L"Multiplication assignment failed for negative vector, positive scalar");
			Assert::AreEqual(mix1 / 2.f, mix3, L"Multiplication assignment failed for mixed vector, positive scalar");

			pos3 = pos1;
			neg3 = neg1;
			mix3 = mix1;
			pos3 /= -2.f;
			neg3 /= -2.f;
			mix3 /= -2.f;
			Assert::AreEqual(pos1 / -2.f, pos3, L"Multiplication assignment failed for positive vector, negative scalar");
			Assert::AreEqual(neg1 / -2.f, neg3, L"Multiplication assignment failed for negative vector, negative scalar");
			Assert::AreEqual(mix1 / -2.f, mix3, L"Multiplication assignment failed for mixed vector, negative scalar");

			// Division by zero
			pos3 = pos1;
			neg3 = neg1;
			mix3 = mix1;
			Assert::ExpectException<Frost::DivByZeroException>([&pos1](void) { return pos1 / 0.f; }, L"Expected exception on divide positive vector by zero");
			Assert::ExpectException<Frost::DivByZeroException>([&neg1](void) { return neg1 / 0.f; }, L"Expected exception on divide negative vector by zero");
			Assert::ExpectException<Frost::DivByZeroException>([&mix1](void) { return mix1 / 0.f; }, L"Expected exception on divide mixed vector by zero");
			Assert::ExpectException<Frost::DivByZeroException>([]() { return Frost::MathConstants::VECTOR_ZERO / 0.f; }, L"Expected exception on divide zero vector by zero");
			Assert::ExpectException<Frost::DivByZeroException>([&pos3](void) { return pos3 /= 0.f; }, L"Expected exception on divide assign positive vector by zero");
			Assert::ExpectException<Frost::DivByZeroException>([&neg3](void) { return neg3 /= 0.f; }, L"Expected exception on divide assign negative vector by zero");
			Assert::ExpectException<Frost::DivByZeroException>([&mix3](void) { return mix3 /= 0.f; }, L"Expected exception on divide assign mixed vector by zero");
			Assert::AreEqual(pos1, pos3, L"Expected unmutated value of positive vector after divide by zero exception");
			Assert::AreEqual(neg1, neg3, L"Expected unmutated value of positive vector after divide by zero exception");
			Assert::AreEqual(mix1, mix3, L"Expected unmutated value of positive vector after divide by zero exception");

			// Against Constants
			Assert::AreEqual(Frost::MathConstants::VECTOR_UNIT_X * 3.f, Frost::Vect3(3.f, 0.f, 0.f), L"Constant unit X vector did not respond as expected to scalar multiplication");
			Assert::AreEqual(Frost::MathConstants::VECTOR_UNIT_Y / 3.f, Frost::Vect3(0.f, 1.f / 3.f, 0.f), L"Constant unit Y vector did not respond as expected to scalar division");
			Assert::AreEqual(Frost::MathConstants::VECTOR_UNIT_Z * 0.f, Frost::MathConstants::VECTOR_ZERO, L"Constant unit Z vector did not respond as expected to multiplication by zero");
			Assert::AreEqual(Frost::MathConstants::VECTOR_ONE * 0.f, Frost::MathConstants::VECTOR_ZERO, L"Constant ones vector did not respond as expected to multiplication by zero");
		}

		TEST_METHOD(accessorMethods)
		{
			// Square brackets access, access and modify
			Frost::Vect3 v1(2.f, 4.f, 6.f);
			Assert::AreEqual(2.f, v1[0], L"Square bracket accessor does not return expected result on first component");
			Assert::AreEqual(4.f, v1[1], L"Square bracket accessor does not return expected result on second component");
			Assert::AreEqual(6.f, v1[2], L"Square bracket accessor does not return expected result on third component");

			v1[0] = 1.f;
			v1[1] = 2.f;
			v1[2] = 3.f;
			Assert::AreEqual(1.f, v1[0], L"Square bracket mutator does not return expected result on first component");
			Assert::AreEqual(2.f, v1[1], L"Square bracket mutator does not return expected result on second component");
			Assert::AreEqual(3.f, v1[2], L"Square bracket mutator does not return expected result on third component");

			// Out of bounds
			try {
				auto foo = v1[-1];
				Assert::Fail(L"Expected index out of bounds exception when accessing -1 element of Vect3");
			}
			catch (Frost::IndexOutOfBoundsException iex) {
				Assert::AreEqual(-1, iex.indexGiven, L"Index provided by IndexOutOfBoundsException not what expected.");
			}
			try {
				auto foo = v1[3];
				Assert::Fail(L"Expected index out of bounds exception when accessing 3 element of Vect3");
			}
			catch (Frost::IndexOutOfBoundsException iex) {
				Assert::AreEqual(3, iex.indexGiven, L"Index provided by IndexOutOfBoundsException not what expected.");
			}

			Frost::Vect3 v2 = v1;
			try {
				v1[-1] = 555.f;
				Assert::Fail(L"Expected index out of bounds exception when accessing -1 element of Vect3");
			}
			catch (Frost::IndexOutOfBoundsException iex) {
				Assert::AreEqual(-1, iex.indexGiven, L"Index provided by IndexOutOfBoundsException not what expected.");
			}

			try {
				v1[3] = 223.f;
				Assert::Fail(L"Expected index out of bounds exception when accessing 3 element of Vect3");
			}
			catch (Frost::IndexOutOfBoundsException iex) {
				Assert::AreEqual(3, iex.indexGiven, L"Index provided by IndexOutOfBoundsException not what expected.");
			}
			Assert::AreEqual(v1, v2, L"On invalid mutate functions (exception thrown), should not actually mutate vector");

			// Constant vector
			const Frost::Vect3 v3(2.f, 3.f, 4.f);
			Assert::AreEqual(2.f, v3[0], L"Square bracket accessor does not return expected result on first component with constant vector");
			Assert::AreEqual(3.f, v3[1], L"Square bracket accessor does not return expected result on second component with constant vector");
			Assert::AreEqual(4.f, v3[2], L"Square bracket accessor does not return expected result on third component with constant vector");

			try {
				auto foo = v3[-1];
				Assert::Fail(L"Expected index out of bounds exception when accessing -1 element of const Vect3");
			}
			catch (Frost::IndexOutOfBoundsException iex) {
				Assert::AreEqual(-1, iex.indexGiven, L"Index provided by IndexOutOfBoundsException not what expected.");
			}
			try {
				auto foo = v3[3];
				Assert::Fail(L"Expected index out of bounds exception when accessing 3 element of const Vect3");
			}
			catch (Frost::IndexOutOfBoundsException iex) {
				Assert::AreEqual(3, iex.indexGiven, L"Index provided by IndexOutOfBoundsException not what expected.");
			}
		}

		TEST_METHOD(vectorFunctions)
		{
			// Scalar Product
			Assert::AreEqual(Frost::Vect3(1.f, 1.f, 1.f) * Frost::Vect3(2.f, 4.f, 6.f), 12.f, L"Scalar product did not yield expected result");
			Assert::AreEqual(Frost::Vect3(1.f, 2.f, 0.f) * Frost::Vect3(0.f, 0.f, 2.f), 0.f, L"Scalar product of orthogonal vectors did not yield expected result");
			Assert::AreEqual(Frost::MathConstants::VECTOR_UNIT_X * Frost::MathConstants::VECTOR_UNIT_Y, 0.f, L"Scalar product of unit x and y vectors did not yield zero");
			Assert::AreEqual(Frost::MathConstants::VECTOR_ZERO * Frost::Vect3(444.f, 555.f, 666.f), 0.f, L"Scalar product of zero vector with other vector did not yield zero");

			// Vector Product
			Assert::AreEqual(Frost::CrossProduct(Frost::MathConstants::VECTOR_UNIT_X, Frost::MathConstants::VECTOR_UNIT_Y), Frost::MathConstants::VECTOR_UNIT_Z, L"Vector product of unit x and unit y vectors does not yield unit z vector");
			Assert::AreEqual(Frost::CrossProduct(Frost::MathConstants::VECTOR_UNIT_Y, Frost::MathConstants::VECTOR_UNIT_X), Frost::MathConstants::VECTOR_UNIT_Z * -1.f, L"Vector product of unit y and unit x vectors does not yield negative unit z vector");
			Assert::AreEqual(Frost::CrossProduct(Frost::MathConstants::VECTOR_ZERO, Frost::MathConstants::VECTOR_ONE), Frost::MathConstants::VECTOR_ZERO, L"Vector product of zero and one vector does not equal zero vector");
			Assert::AreEqual(Frost::Vect3(Frost::Vect3Normal(Frost::CrossProduct(Frost::MathConstants::VECTOR_UNIT_X,
				Frost::Vect3Normal(Frost::MathConstants::VECTOR_UNIT_X + Frost::MathConstants::VECTOR_UNIT_Y)))), Frost::MathConstants::VECTOR_UNIT_Z,
				L"Vector product of two unit vectors non orthogonal does not yield expected result");

			// Magnitude, Square Magnitude
			Assert::AreEqual(1.f, Frost::MathConstants::VECTOR_UNIT_X.magnitude(), L"Unit x vector does not report magnitude of 1.0");
			Assert::AreEqual(1.f, Frost::MathConstants::VECTOR_UNIT_Y.magnitude(), L"Unit y vector does not report magnitude of 1.0");
			Assert::AreEqual(1.f, Frost::MathConstants::VECTOR_UNIT_Z.magnitude(), L"Unit z vector does not report magnitude of 1.0");
			Assert::AreEqual(0.f, Frost::MathConstants::VECTOR_ZERO.magnitude(), L"Zero vector does not report magnitude of 0.0");
			Assert::AreEqual(1.f, Frost::MathConstants::VECTOR_UNIT_X.squareMagnitude(), L"Unit x vector does not report square magnitude of 1.0");
			Assert::AreEqual(1.f, Frost::MathConstants::VECTOR_UNIT_Y.squareMagnitude(), L"Unit y vector does not report square magnitude of 1.0");
			Assert::AreEqual(1.f, Frost::MathConstants::VECTOR_UNIT_Z.squareMagnitude(), L"Unit z vector does not report square magnitude of 1.0");
			Assert::AreEqual(0.f, Frost::MathConstants::VECTOR_ZERO.magnitude(), 0.f, L"Zero vector does not report square magnitude of 0.0");

			// Component Product
			Assert::AreEqual(Frost::Vect3(1.f, 0.f, 0.f), Frost::ComponentProduct(Frost::MathConstants::VECTOR_ONE, Frost::MathConstants::VECTOR_UNIT_X), L"Component product on ones vector and unit x vector did not yield expected result");
			Assert::AreEqual(Frost::Vect3(3.f, 4.f, 6.f), Frost::ComponentProduct({ 1.f, 2.f, 3.f }, { 3.f, 2.f, 2.f }), L"Component product on arbitrary vectors failed");
		}

		TEST_METHOD(approximationTests)
		{
			// Equality Tests
			Frost::Vect3 zero_vector(0.f, 0.f, 0.f);
			Assert::IsTrue(zero_vector == Frost::MathConstants::VECTOR_ZERO, L"Failed to equate zero vectors");
			Assert::IsFalse(zero_vector != Frost::MathConstants::VECTOR_ZERO, L"Failed to reject inequality of zero vectors");

			Frost::Vect3 ones_vector(1.f, 1.f, 1.f);
			Assert::IsTrue(ones_vector == Frost::MathConstants::VECTOR_ONE, L"Failed to equate ones vectors");
			Assert::IsFalse(ones_vector != Frost::MathConstants::VECTOR_ONE, L"Failed to reject inequality of ones vectors");

			Assert::IsFalse(Frost::MathConstants::VECTOR_UNIT_X == Frost::MathConstants::VECTOR_UNIT_Y, L"Equated unit x and unit y vectors");
			Assert::IsFalse(Frost::MathConstants::VECTOR_UNIT_X == Frost::MathConstants::VECTOR_UNIT_Z, L"Equated unit x and unit z vectors");
			Assert::IsFalse(Frost::MathConstants::VECTOR_ONE == Frost::MathConstants::VECTOR_UNIT_Y, L"Equated ones vector to unit y vector");
			Assert::IsFalse(Frost::MathConstants::VECTOR_ZERO == Frost::MathConstants::VECTOR_ONE, L"Equated ones vector to zero vector");

			// isApproximately functions
			Frost::Vect3 testApproximation(1.999f, 2.001f, 2.f);
			Assert::IsTrue(testApproximation.isApproximately({ 2.f, 2.f, 2.f }, 0.01f), L"isApproximately test failed on large margin of error");
			Assert::IsTrue(testApproximation.isApproximately({ 2.f, 2.f, 2.f }, 0.002f), L"isApproximately test failed for exact epsilon");
			Assert::IsFalse(testApproximation.isApproximately({ 2.f, 2.f, 2.f }, 0.00001f), L"isApproximation test succeeded when should have failed on epsilon too small");
			Assert::IsTrue(testApproximation.isApproximately({ 2.f, 2.f, 2.f }, { 0.1f, 0.1f, 0.1f }), L"isApproximately test failed on large margin of error");
			Assert::IsTrue(testApproximation.isApproximately({ 2.f, 2.f, 2.f }, { 0.0010001f, 0.0010001f, 0.0000001f }), L"isApproximately test failed for (nearly) exact epsilon");
			Assert::IsFalse(testApproximation.isApproximately({ 2.f, 2.f, 2.f }, { 0.00001f, 0.00001f, 0.00001f }), L"isApproximation test succeeded when should have failed on epsilon too small");

			// Negative Numbers
			testApproximation *= -1.f;
			Assert::IsTrue(testApproximation.isApproximately({ -2.f, -2.f, -2.f }, 0.01f), L"isApproximately test failed on large margin of error with negative vector");
			Assert::IsTrue(testApproximation.isApproximately({ -2.f, -2.f, -2.f }, 0.002f), L"isApproximately test failed for exact epsilon with negative vector");
			Assert::IsFalse(testApproximation.isApproximately({ -2.f, -2.f, -2.f }, 0.00001f), L"isApproximation test succeeded when should have failed on negative vector, epsilon too small");
			Assert::IsTrue(testApproximation.isApproximately({ -2.f, -2.f, -2.f }, { 0.01f, 0.01f, 0.01f }), L"isApproximately test failed on large margin of error with negative vector");
			Assert::IsTrue(testApproximation.isApproximately({ -2.f, -2.f, -2.f }, { 0.0010001f, 0.0010001f, 0.0000001f }), L"isApproximately test failed for (nearly) exact epsilon with negative vector");
			Assert::IsFalse(testApproximation.isApproximately({ -2.f, -2.f, -2.f }, { 0.00001f, 0.00001f, 0.00001f }), L"isApproximation test succeeded when should have failed on negative vector, epsilon too small");
		}
	};
}
