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
#include "WorldManager.h"
#include <thread>

namespace Frost
{

int WorldManager::nInstances = 0;

WorldManager::WorldManager()
: _collisionManager(0)
, _forces()
, _allManagedObjects()
, _masterContactList(0)
{
	if (nInstances > 0)
		throw DuplicateActionException();

	nInstances++;
}

WorldManager::WorldManager(FROST_COLLISION_MANAGER cmt)
: _collisionManager(0)
, _forces()
, _allManagedObjects()
, _masterContactList(0)

{
	if (nInstances > 0)
		throw DuplicateActionException();

	switch (cmt)
	{
	case FROST_COLLISION_MANAGER_BVHTREE:
		_collisionManager = std::shared_ptr<ICollisionManager>(new BVHTree());
		break;
	case FROST_COLLISION_MANAGER_BRUTE_FORCE:
		_collisionManager = std::shared_ptr<ICollisionManager>(new BruteForceCollisionManager());
		break;
	default:
		throw NotImplementedException();
	}

	nInstances++;
}

WorldManager::~WorldManager()
{
	while (_masterContactList.size() > 0)
	{
		_masterContactList.erase(_masterContactList.begin());
	}
	if (_collisionManager != 0)
	{
		_collisionManager.reset();
	}
	while (this->_allManagedObjects.size() > 0)
	{
		_allManagedObjects.erase(_allManagedObjects.begin());
	}
	_forces.clearRegistry();
	nInstances--;
}

void WorldManager::update(float timeElapsed)
{
	if (_collisionManager == 0)
	{
		throw NotImplementedException();
	}

	_collisionManager->update(timeElapsed);
	_forces.updateForces(timeElapsed);

	// Important: With the current implementation (I say that because I don't like it),
	//  contacts must be resolved before physics objects can be updated. This is because
	//  contacts may add forces to the object, and may look at forces in the object.
	_collisionManager->genContacts(_masterContactList);

	// TODO: Figure this out...
	//  There are things you don't want to do if no time has elapsed.
	//  You should implement this instead at the Collidable level.
	if (timeElapsed == 0) return;

	// TODO: In the future, you should always have a thread running that updates
	//  all this, and then just query it at the frame.
	// Right now, just handle all contacts.
	while (_masterContactList.size() > 0u)
	{
		_masterContactList[0u]->resolve(timeElapsed);
		_masterContactList.erase(_masterContactList.begin());
	}

	for (auto i = _allManagedObjects.begin(); i < _allManagedObjects.end(); ++i)
	{
		(*i)->getPhysicsObject()->update(timeElapsed);
	}
}

std::shared_ptr<IPhysicsNode> WorldManager::addObject(std::shared_ptr<IPhysicsObject> objectToAdd, std::shared_ptr<Collidable> collisionData, std::string name)
{
	// Make sure the name does not exist...
	for (auto i = _allManagedObjects.begin(); i < _allManagedObjects.end(); ++i)
	{
		if ((*i)->getName() == name)
		{
			throw DuplicateActionException();
		}
	}

	// TODO: Change this whole function to not even take in collision data as a parameter,
	//  instead attach it later to physics objects if desired. At least overload it that way.
	if (collisionData != 0 && collisionData->getAttachedObject() == 0)
	{
		collisionData->attachObject(objectToAdd);
	}

	std::shared_ptr<IPhysicsNode> nodeToAdd = std::make_shared<IPhysicsNode>(objectToAdd, collisionData, name);
	if (collisionData != 0)
	{
		_collisionManager->addPhysicsNode(nodeToAdd);
	}

	_allManagedObjects.push_back(nodeToAdd);

	return nodeToAdd;
}

std::shared_ptr<IPhysicsNode> WorldManager::addObject(std::shared_ptr<IPhysicsObject> objectToAdd, std::string name)
{
	// Make sure that the name does not exist...
	for (auto i = _allManagedObjects.begin(); i < _allManagedObjects.end(); ++i)
	{
		if ((*i)->getName() == name)
		{
			throw DuplicateActionException();
		}
	}

	auto collisionData = std::make_shared<Collidable>(objectToAdd);

	std::shared_ptr<IPhysicsNode> nodeToAdd = std::make_shared<IPhysicsNode>(objectToAdd, collisionData, name);
	_collisionManager->addPhysicsNode(nodeToAdd);

	_allManagedObjects.push_back(nodeToAdd);

	return nodeToAdd;
}

std::shared_ptr<IPhysicsNode> WorldManager::getObjectByName(std::string name)
{
	if (name == "")
	{
		throw ObjectDoesNotExistException(name);
	}

	for (auto i = _allManagedObjects.begin(); i < _allManagedObjects.end(); ++i)
	{
		if ((*i)->getName() == name)
		{
			return (*i);
		}
	}

	throw ObjectDoesNotExistException(name);
}

std::shared_ptr<IPhysicsNode> WorldManager::operator[](std::string name)
{
	return getObjectByName(name);
}

void WorldManager::addForce(std::shared_ptr<IForce> f, std::shared_ptr<IPhysicsNode> o)
{
	_forces.add(f, o);
}

void WorldManager::addForce(std::shared_ptr<IForce> forceToAdd, std::string o)
{
	_forces.add(forceToAdd, getObjectByName(o));
}

void WorldManager::attachCollisionManager(std::shared_ptr<ICollisionManager> t)
{
	if (_collisionManager == 0)
	{
		_collisionManager = t;
	}
	else
	{
		throw DuplicateActionException();
	}
}
}