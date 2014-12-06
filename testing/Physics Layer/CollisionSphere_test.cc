#include "CollisionSphere_test.h"

namespace CollisionSphereTests
{
	TestResult ConstructionTests()
	{
		Frost::CollisionSphere unit_sphere(1.f, { 0.f, 0.f, 0.f }, nullptr);
		if (unit_sphere.getRadius() != 1.f)
			return TestResult(false, "Failed to initialize unit sphere with radius 1");
		if (unit_sphere.getPos() != Frost::MathConstants::VECTOR_ZERO)
			return TestResult(false, "Failed to place unit sphere at origin");
		if (unit_sphere.getAttachedObjectPtr() != nullptr)
			return TestResult(false, "Failed to attach nullptr to unit collision sphere");
		if (unit_sphere.getTransformMatrix() == Frost::MathConstants::MATRIX_IDENTITY)
			return TestResult(false, "Unit sphere at origin does not use identity matrix as transform", Frost::MathConstants::MATRIX_IDENTITY, unit_sphere.getTransformMatrix());

		// Todo: Something else.
		// TODO: Test attachment of IPhysicsObject instances

		return TEST_PASSED;
	}

	TestResult IsTouchingSphereTests()
	{
		std::vector<std::shared_ptr<Frost::IContact>> o_contacts(0);

		// Touching, but not penetrating
		Frost::CollisionSphere s1(1.f, { -1.f, 0.f, 0.f }, nullptr), s2(1.f, { 1.f, 0.f, 0.f }, nullptr);
		if (!s1.isTouching(s2)) return TestResult(false, "Unit spheres failed isTouching test on touching");
		s1.genContacts(s2, o_contacts);
		if (o_contacts.size() > 1u) return TestResult(false, "Too many contacts generated for touching unit spheres");
		else if (o_contacts.size() == 0u) return TestResult(false, "No contacts generated for touching unit spheres");
		else
		{
			auto contact = o_contacts[0];
			if (contact->getCollisionPoint() != Frost::MathConstants::VECTOR_ZERO)
			{
				return TestResult(false, "Collision point off for two unit vectors", Frost::MathConstants::VECTOR_ZERO, contact->getCollisionPoint());
			}
		}

		// Penetrating
		// Non-penetrating
		// Large spheres (penetrating, non-penetrating, touching)
		// Small spheres (penetrating, non-penetrating, touching)

		return TEST_PASSED;
	}

	TestResult IsTouchingBoxTests()
	{
		return TEST_PASSED;
	}

	TestResult AccessorTests()
	{
		return TEST_PASSED;
	}

}