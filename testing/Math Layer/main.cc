#include "Vect3_test.h"
#include <iostream>

inline void AnnounceTestGroup(std::string unitBeingTested)
{
	std::cout << "-----" << unitBeingTested << "-----" << std::endl;
}

inline void ConductTest(TestResult(*test)(), std::string testName)
{
	std::cout << testName << ": ";
	TestResult res = test();
	if (!res.wasSuccess())
	{
		std::cout << "FAIL: " << res.toString() << std::endl;
		std::cin.ignore();
		exit(0);
	}
	else
	{
		std::cout << "PASS" << std::endl;
	}
}

int main()
{
	AnnounceTestGroup("Vect3");
	ConductTest(Vect3Tests::ConstructorTests, "Ctor");
	ConductTest(Vect3Tests::AccessorMethodTests, "Accessors");
	ConductTest(Vect3Tests::VectorArithmeticTests, "Arithmetic");
	ConductTest(Vect3Tests::ScalarTests, "Scalar Multiplication");
	ConductTest(Vect3Tests::VectorFunctionTests, "Vector Functions");
	ConductTest(Vect3Tests::ApproximationTests, "Approximation");
	std::cout << "\n--------------------------------\nAll tests passed!\n--------------------------------";
	std::cin.ignore();
	return 0;
}