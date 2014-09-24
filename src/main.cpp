#include <iostream>
#include <iomanip>
#include <random>
#include <ctime>
#include <chrono>
#include "FrostWorldMan.h"

const float LO = -25.f;
const float HI = 25.f;
#define RAND_FLOAT LO + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX/(HI-LO)))

std::ostream& operator<<(std::ostream& o, const Frost::Vect3& v)
{
	return o << "<" << v._x << ", " << v._y << ", " << v._z << ">";
}

std::ostream& operator<<(std::ostream& o, const Frost::Quaternion& q)
{
	return o << "[" << q._w << " + " << q._x << "i + " << q._y << "j + " << q._z << "k]";
}

std::ostream& operator<<(std::ostream& o, const Frost::Matrix& m)
{
	return o
		<< m.m[0][0] << ", " << m.m[0][1] << ", " << m.m[0][2] << ", " << m.m[0][3] << "\n"
		<< m.m[1][0] << ", " << m.m[1][1] << ", " << m.m[1][2] << ", " << m.m[1][3] << "\n"
		<< m.m[2][0] << ", " << m.m[2][1] << ", " << m.m[2][2] << ", " << m.m[2][3] << "\n"
		<< m.m[3][0] << ", " << m.m[3][1] << ", " << m.m[3][2] << ", " << m.m[3][3] << "\n";
}

std::ostream& operator<<(std::ostream& o, const Frost::BoundingSphere& b)
{
	return o << "-------BOUNDING SPHERE------------" << std::endl
		<< "Position: " << b.getPos() << std::endl
		<< "Radius: " << b.getRadius() << std::endl;
}

int main()
{
	Frost::WorldManager root;
	root.attachCollisionManager(new Frost::BVHTree);

	root.addObject(new Frost::TestPhysicsObject(1.f, Frost::MathConstants::MATRIX_IDENTITY,
		{ 1.f, 0.f, 0.f }, Frost::MathConstants::QUATERNION_UNIT,
		{ 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f }),
		new Frost::Collidable(),
		"Sphere1");
	root.getObjectByName("Sphere1")->getCollidableData()->addCollisionObject(new Frost::CollisionSphere(1.f), Frost::MathConstants::MATRIX_IDENTITY);
	root.addObject(new Frost::TestPhysicsObject(1.f, Frost::MathConstants::MATRIX_IDENTITY,
		{ -1.f, 0.f, 0.f }, Frost::MathConstants::QUATERNION_UNIT,
		{ 0.f, 0.f, 0.f }, { 0.f, 0.f, 0.f }),
		new Frost::Collidable(),
		"Sphere2");
	root.getObjectByName("Sphere2")->getCollidableData()->addCollisionObject(new Frost::CollisionSphere(1.f), Frost::MathConstants::MATRIX_IDENTITY);

	auto now = std::chrono::system_clock::now();
	for (int i = 0; i < 50; i++)
	{
		root.update(0.001f);
	}
	auto later = std::chrono::system_clock::now();

	std::cout << "Operation finished in:" << std::endl;
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(later - now).count() << " milliseconds" << std::endl;
	std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(later - now).count() << " nanoseconds" << std::endl;

	return 0;
}