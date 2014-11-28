/*
This source file is part of the Indigo Frost physics engine

The MIT License (MIT)

Copyright (c) 2014 Kamaron Peterson (aggieblue92)

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef FROST_WORLD_MANAGER_H
#define FROST_WORLD_MANAGER_H

/////////////////////////////////////////
// WorldManager: This is the physics engine!
//  Everything is wrapped in here. Yep.
/////////////////////////////////////////

#include <memory>
#include <sstream>
#include <fstream>
#include <string>
#include "FrostPhysics.h"
#include "ForceRegistry.h"
#include "IForce.h"
#include "IPhysicsNode.h"
#include "ICollisionManager.h"
#include "BVHTree.h"
#include "BruteForceCollisionManager.h"
#include "DebugLogger.h"

namespace Frost
{
	class ObjectDoesNotExistException
	{
	public:
		std::string name;
		ObjectDoesNotExistException(std::string n) : name(n) {}
	};

	class FileNotFoundException
	{
	public:
		std::string fileName;
		FileNotFoundException(std::string fn) : fileName(fn) {}
	};

	// As different collision management schemes are added to the engine,
	//  include different collision manager types here.
	enum FROST_COLLISION_MANAGER
	{
		FROST_COLLISION_MANAGER_BRUTE_FORCE,
		FROST_COLLISION_MANAGER_BVHTREE
	};

	class WorldManager
	{
	public:
		WorldManager();
		WorldManager(FROST_COLLISION_MANAGER collisionManagerType);
		~WorldManager();

		// Another moment ticks by in eternity...
		virtual void update(float timeElapsed);

		// Add an object to the managed system.
		std::shared_ptr<IPhysicsNode> addObject(std::shared_ptr<IPhysicsObject> objectToAdd, std::shared_ptr<Collidable> collisionData, std::string name = "");
		std::shared_ptr<IPhysicsNode> addObject(std::shared_ptr<IPhysicsObject> objectToAdd, std::string name = "");

		// Get an object, by name, from the managed system
		std::shared_ptr<IPhysicsNode> getObjectByName(std::string name);

		std::shared_ptr<IPhysicsNode> operator[](std::string name);

		// Add a force to the system, to an object
		void addForce(std::shared_ptr<IForce> forceToAdd, std::shared_ptr<IPhysicsNode> objectToAffect);

		// Add a force to tye system, to an object by string name
		void addForce(std::shared_ptr<IForce> forceToAdd, std::string objectToAffect);

		// Attach a collision manager to the world manager class
		// TODO: Replace this with a constructor.
		void attachCollisionManager(std::shared_ptr<ICollisionManager> toAttach);

	protected:
		std::shared_ptr<ICollisionManager> _collisionManager;
		ForceRegistry _forces;
		std::vector<std::shared_ptr<IPhysicsNode>> _allManagedObjects;

	private:
		static int nInstances;
		std::vector<std::shared_ptr<IContact>> _masterContactList;
	};
}

#endif