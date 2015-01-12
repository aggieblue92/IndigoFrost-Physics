#include "stdafx.h"
#include "CppUnitTest.h"
#include <iomanip>
#include <FrostMath.h>
#include <CollisionSphere.h>

namespace Frost
{
	namespace Testing
	{
		const float EPSILON = 0.0001f;
	}
}

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

			template<> static std::wstring ToString<Frost::Vect3Normal>(const Frost::Vect3Normal& vn)
			{
				std::wstringstream wss(L"");
				wss << L"[" << vn._x << L", " << vn._y << ", " << vn._z << "]";
				return wss.str();
			}

			template<> static std::wstring ToString<Frost::Matrix>(const Frost::Matrix& m)
			{
				std::wstringstream wss(L"");
				
				wss << std::endl;
				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 4; j++)
					{
						wss << std::left << std::setw(5) << m.m[i][j] << "  ";
					}
					wss << std::endl;
				}
				wss << std::endl;

				return wss.str();
			}
		}
	}
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(CollisionSphere_test)
	{
	public:
		
		TEST_METHOD(ctor)
		{
			Frost::CollisionSphere unit_sphere(1.f, { 0.f, 0.f, 0.f }, nullptr);
			Assert::AreEqual(unit_sphere.getRadius(), 1.f, L"Unit sphere reports incorrect radius");
			Assert::AreEqual(unit_sphere.getPos(), Frost::MathConstants::VECTOR_ZERO, L"Unit sphere not placed at origin");
			Assert::IsNull(unit_sphere.getAttachedObjectPtr().get(), L"Failed to attach null pointer to unit collision sphere");
			Assert::AreEqual(unit_sphere.getTransformMatrix(), Frost::MathConstants::MATRIX_IDENTITY, L"Unit sphere at origin does not use identity matrix as transform");

			// TODO: Test attachment of IPhysicsObject instances
		}

		TEST_METHOD(isTouching_sphere)
		{
			// Touching, not penetrating
			Frost::CollisionSphere s1(1.f, { -1.f, 0.f, 0.f }, nullptr), s2(1.f, { 1.f, 0.f, 0.f }, nullptr);
			Assert::IsTrue(s1.isTouching(s2), L"Unit spheres failed isTouching test on touching");
			Assert::IsTrue(s2.isTouching(s1), L"Unit spheres failed isTouching test on touching");

			// Penetrating
			s1 = Frost::CollisionSphere(50.f, { 15.f, 10.f, 1.2f }, nullptr);
			s2 = Frost::CollisionSphere(22.f, { -7.5f, 22.2f, -18.8f }, nullptr);
			Assert::IsTrue(s1.isTouching(s2), L"Arbitrary spheres failed isTouching on penetrating");
			Assert::IsTrue(s2.isTouching(s1), L"Arbitrary spheres failed isTouching on penetrating");

			// Not touching or penetrating
			s1 = Frost::CollisionSphere(22.f, { 90.f, 0.f, 0.f }, nullptr);
			s2 = Frost::CollisionSphere(45.f, { -2.f, 11.f, 2.f }, nullptr);
			Assert::IsFalse(s1.isTouching(s2), L"Arbitrary spheres falsely reported to be in contact with each other");
			Assert::IsFalse(s2.isTouching(s1), L"Arbitrary spheres falsely reported to be in contact with each other");
		}

		TEST_METHOD(genContacts_sphere)
		{
			std::vector<std::shared_ptr<Frost::IContact>> o_contacts(0);
			
			// Touching, not penetrating
			Frost::CollisionSphere s1(1.f, { -1.f, 0.f, 0.f }, nullptr), s2(1.f, { 1.f, 0.f, 0.f }, nullptr);
			s1.genContacts(s2, o_contacts);
			Assert::AreEqual(o_contacts.size(), 2u, L"Improper number of contacts generated for touching unit spheres");

			auto contact = o_contacts[0];
			auto contact2 = o_contacts[1];
			Assert::AreEqual(contact->getCollisionPoint(), Frost::MathConstants::VECTOR_ZERO, L"Collision point off for two unit spheres");
			Assert::AreEqual(contact2->getCollisionPoint(), Frost::MathConstants::VECTOR_ZERO, L"Second collision point off for two unit spheres");
			Assert::AreEqual(contact->getContactMagnitude(), 0.f, L"Contact magnitude for touching (no penetration) incorrect on first contact");
			Assert::AreEqual(contact2->getContactMagnitude(), 0.f, L"Contact magnitude for touching (no penetration) incorrect on first contact");

			auto dir1 = contact->getContactNormal();
			auto dir2 = contact2->getContactNormal();
			Assert::IsTrue(((contact->getContactNormal() == Frost::MathConstants::VECTOR_UNIT_X && contact2->getContactNormal() == Frost::MathConstants::VECTOR_UNIT_X * -1.f)
				|| (contact2->getContactNormal() == Frost::MathConstants::VECTOR_UNIT_X && contact->getContactNormal() == Frost::MathConstants::VECTOR_UNIT_X * -1.f)),
				L"Penetration direction for aligned unit spheres not equal to unit x axis");

			// Penetrating
			o_contacts.clear();
			s1 = Frost::CollisionSphere(50.f, { 15.f, 10.f, 1.2f }, nullptr);
			s2 = Frost::CollisionSphere(22.f, { -7.5f, 22.2f, -18.8f }, nullptr);
			s1.genContacts(s2, o_contacts);
			Assert::AreEqual(o_contacts.size(), 2u, L"Improper number of contacts generated for touching unit spheres");
			contact = o_contacts[0];
			contact2 = o_contacts[1];

			Frost::Vect3 distanceApart = s2.getPos() - s1.getPos();
			Assert::IsTrue((distanceApart - Frost::Vect3(-22.5f, 12.2f, -20.f)).squareMagnitude() < Frost::Testing::EPSILON, L"Unexpected distance apart vector");
			Frost::Vect3 interpenetration = Frost::Vect3Normal(distanceApart) * (-distanceApart.magnitude() + s1.getRadius() + s2.getRadius());
			Frost::Vect3 interpenetration_midpoint = ((s1.getPos() + Frost::Vect3Normal(distanceApart) * s1.getRadius()) + (s2.getPos() - Frost::Vect3Normal(distanceApart) * s2.getRadius())) / 2.f;

			Assert::AreEqual(contact->getCollisionPoint(), contact2->getCollisionPoint(), L"Collision points generated are not the same");
			Assert::IsTrue((contact->getCollisionPoint() - interpenetration_midpoint).squareMagnitude() < Frost::Testing::EPSILON, L"Incorrect interpenetration midpoint on first contact");
			Assert::IsTrue((contact2->getCollisionPoint() - interpenetration_midpoint).squareMagnitude() < Frost::Testing::EPSILON, L"Incorrect interpenetration midpoint on second contact");

			Assert::IsTrue(contact->getContactMagnitude() - interpenetration.magnitude() < Frost::Testing::EPSILON, L"Incorrect interpenetration magnitude on first contact");
			Assert::IsTrue(contact2->getContactMagnitude() - interpenetration.magnitude() < Frost::Testing::EPSILON, L"Incorrect interpenetration magnitude on second contact");

			Assert::IsTrue(contact->getContactNormal().isApproximately(Frost::Vect3Normal(interpenetration), Frost::Testing::EPSILON)
				|| contact->getContactNormal().isApproximately(Frost::Vect3Normal(interpenetration * -1.f), Frost::Testing::EPSILON), L"Contact normal on first contact inaccurate");
			Assert::IsTrue(contact2->getContactNormal().isApproximately(Frost::Vect3Normal(interpenetration), Frost::Testing::EPSILON)
				|| contact2->getContactNormal().isApproximately(Frost::Vect3Normal(interpenetration * -1.f), Frost::Testing::EPSILON), L"Contact normal on second contact inaccurate");

			// Non-penetrating
			o_contacts.clear();
			s1 = Frost::CollisionSphere(22.f, { 90.f, 0.f, 0.f }, nullptr);
			s2 = Frost::CollisionSphere(45.f, { -2.f, 11.f, 2.f }, nullptr);
			s1.genContacts(s2, o_contacts);
			Assert::AreEqual(o_contacts.size(), 0u, L"Contacts generated when spheres were not touching");
			
			o_contacts.clear();
			s1 = Frost::CollisionSphere(1.f, { -1.f, 0.f, 0.f }, nullptr);
			s2 = Frost::CollisionSphere(0.999f, { 1.f, 0.f, 0.f }, nullptr);
			s1.genContacts(s2, o_contacts);
			Assert::AreEqual(o_contacts.size(), 0u, L"Contacts generated when spheres were barely not touching");
		}

	};
}