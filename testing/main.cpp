#include <FrostWorldMan.h>
#include <iostream>
using namespace std;

const float TOTAL_TIME = 20.f;
const float FRAME_TIME = 1.f / 30.f;

int main()
{
	Frost::DebugLogger::createLogger(Frost::DebugLogger::DEBUG_LEVEL_ALL, "log.txt");

	Frost::WorldManager root(Frost::FROST_COLLISION_MANAGER_BRUTE_FORCE);

	// Add objects to scene...
	root.addObject(std::make_shared<Frost::BasicPhysicsObject>(10.f, Frost::MathConstants::MATRIX_IDENTITY,
		0.f, 0.f, Frost::Vect3(0.f, 150.f, 400.f)),
		"Ball");
	root.addObject(std::make_shared<Frost::BasicPhysicsObject>(0.f, Frost::MathConstants::MATRIX_ZERO,
		1.f, 1.f, Frost::Vect3(0.f, 0.f, 400.f)),
		"Table");

	// Add collision information to the objects in our scene
	root["Ball"]->getCollidableData()->addCollisionObject(
		std::make_shared<Frost::CollisionSphere>(10.f, root["Ball"]->getPhysicsObject()->getPos(), root["Ball"]->getPhysicsObject()),
		Frost::MathConstants::MATRIX_IDENTITY);
	root["Table"]->getCollidableData()->addCollisionObject(
		std::make_shared<Frost::CollisionBox>(Frost::Vect3(130.f, 10.f, 130.f), root["Table"]->getPhysicsObject()->getPos(),
		Frost::MathConstants::QUATERNION_UNIT, root["Table"]->getPhysicsObject()),
		Frost::MathConstants::MATRIX_IDENTITY);

	// Add forces to the objects in our scene
	root.addForce(std::make_shared<Frost::GravityForce>(Frost::MathConstants::VECTOR_UNIT_Y * -1.f, 98.1f), "Ball");

	for(float i = 0.f; i < TOTAL_TIME; i += FRAME_TIME)
	{
		root.update(FRAME_TIME);
	}

	return 0;
}