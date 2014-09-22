#ifndef FROST_WORLD_MANAGER_H
#define FROST_WORLD_MANAGER_H

/////////////////////////////////////////
// WorldManager: This is the physics engine!
//  Everything is wrapped in here. Yep.
/////////////////////////////////////////

#include "FrostPhysics.h"
#include "ForceRegistry.h"
#include "IForce.h"
#include "IPhysicsNode.h"
#include "ICollisionManager.h"

namespace Frost
{
	class ObjectDoesNotExistException
	{
	public:
		std::string name;
		ObjectDoesNotExistException(std::string n) : name(n) {}
	};

	class WorldManager
	{
	public:
		WorldManager();
		~WorldManager();

		// Another moment ticks by in eternity...
		virtual void update(float timeElapsed);

		// Add an object to the managed system.
		IPhysicsNode* addObject(IPhysicsObject* objectToAdd, std::string name = "");

		// Get an object, by name, from the managed system
		IPhysicsNode* getObjectByName(std::string name);

		// Add a force to the system, to an object
		void addForce(IForce* forceToAdd, IPhysicsNode* objectToAffect);

		// Add a force to tye system, to an object by string name
		void addForce(IForce* forceToAdd, std::string objectToAffect);

		// Attach a collision manager to the world manager class
		void attachCollisionManager(ICollisionManager* toAttach);
		// void attachCollisionManager(COLLISION_MANAGER_TYPE typeToCreate);

	protected:
		ICollisionManager* _collisionManager;
		ForceRegistry _forces;
		std::vector<IPhysicsNode*> _allManagedObjects;

	private:
		static int nInstances;
		std::vector<IContact*> _masterContactList;
	};
}

#endif