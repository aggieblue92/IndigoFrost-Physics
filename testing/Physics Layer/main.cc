#include "CollisionSphere_test.h"
#include <iostream>

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
	ConductTest(ConstructionTest, "CollisionSphere ctor");
	std::cout << "\n--------------------------------\nAll tests passed!\n--------------------------------";
	std::cin.ignore();
	return 0;
}