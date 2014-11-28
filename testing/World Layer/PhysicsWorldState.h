#ifndef INDIGOFROST_PHYSICSTESTING_WORLDSTATE_H_
#define INDIGOFROST_PHYSICSTESTING_WORLDSTATE_H_

#include <FrostPhysics.h>

struct PhysicsWorldState
{
	std::vector<Frost::IPhysicsObject> _physicsObjects;
	std::vector<Frost::Collidable> _collidables;
	std::vector<Frost::ICollisionGeometry> _collisionGeometry;
};

#endif