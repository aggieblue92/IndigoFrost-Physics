#include "CollisionSphere_test.h"
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
	AnnounceTestGroup("Collision Sphere");
	ConductTest(CollisionSphereTests::ConstructionTests, "Construction");
	ConductTest(CollisionSphereTests::AccessorTests, "Accessors");
	ConductTest(CollisionSphereTests::IsTouchingBoxTests, "Box Collision");
	ConductTest(CollisionSphereTests::IsTouchingSphereTests, "Sphere Collision");
	std::cout << "\n--------------------------------\nAll tests passed!\n--------------------------------";
	std::cin.ignore();
	return 0;
}