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
using namespace Frost;

int WorldManager::nInstances = 0;

WorldManager::WorldManager()
: _masterContactList(0)
, _internalForces()
, _externalForces()
, _collisionManager(0)
{
	if (nInstances > 0)
		throw DuplicateActionException();

	nInstances++;
}

WorldManager::WorldManager(FROST_COLLISION_MANAGER cmt)
: _masterContactList(0)
, _internalForces()
, _externalForces()
, _collisionManager(0)
{
	if (nInstances > 0)
		throw DuplicateActionException();

	switch (cmt)
	{
	case FROST_COLLISION_MANAGER_BVHTREE:
		_collisionManager = new BVHTree();
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
		delete *_masterContactList.begin();
		*_masterContactList.begin() = 0;
		_masterContactList.erase(_masterContactList.begin());
	}
	if (_collisionManager != 0)
	{
		delete _collisionManager;
		_collisionManager = 0;
	}
	while (this->_allManagedObjects.size() > 0)
	{
		delete *_allManagedObjects.begin();
		*_allManagedObjects.begin() = 0;
		_allManagedObjects.erase(_allManagedObjects.begin());
	}
	for (unsigned int i = 0u; i < _internalForces.size(); ++i)
	{
		delete _internalForces[i]._force;
		_internalForces[i]._force = 0;
	}
	_internalForces.clearRegistry();
	nInstances--;
}

void WorldManager::update(float timeElapsed)
{
	if (_collisionManager == 0)
	{
		throw NotImplementedException();
	}

	_collisionManager->update(timeElapsed);
	_internalForces.updateForces(timeElapsed);
	_externalForces.updateForces(timeElapsed);

	// Important: With the current implementation (I say that because I don't like it),
	//  contacts must be resolved before physics objects can be updated. This is because
	//  contacts may add forces to the object, and may look at forces in the object.
	_collisionManager->genContacts(_masterContactList);

	// TODO: In the future, you should always have a thread running that updates
	//  all this, and then just query it at the frame.
	// Right now, just handle all contacts.
	while (_masterContactList.size() > 0u)
	{
		_masterContactList[0u]->resolve(timeElapsed);
		delete _masterContactList[0u];
		_masterContactList[0u] = 0;
		_masterContactList.erase(_masterContactList.begin());
	}

	for (auto i = _allManagedObjects.begin(); i < _allManagedObjects.end(); ++i)
	{
		(*i)->getPhysicsObject()->update(timeElapsed);
	}
}

IPhysicsNode* WorldManager::addObject(IPhysicsObject* objectToAdd, Collidable* collisionData, std::string name)
{
	// Make sure the name does not exist...
	for (auto i = _allManagedObjects.begin(); i < _allManagedObjects.end(); ++i)
	{
		if ((*i)->getName() == name)
		{
			throw DuplicateActionException();
		}
	}

	if (collisionData != 0 && collisionData->getAttachedObject() == 0)
	{
		collisionData->attachObject(objectToAdd);
	}

	IPhysicsNode* nodeToAdd = new IPhysicsNode(objectToAdd, collisionData, name);
	if (collisionData != 0)
	{
		_collisionManager->addPhysicsNode(nodeToAdd);
	}

	_allManagedObjects.push_back(nodeToAdd);

	return nodeToAdd;
}

IPhysicsNode* WorldManager::getObjectByName(std::string name)
{
	for (auto i = _allManagedObjects.begin(); i < _allManagedObjects.end(); ++i)
	{
		if ((*i)->getName() == name)
		{
			return (*i);
		}
	}

	throw ObjectDoesNotExistException(name);
}

IPhysicsNode* WorldManager::operator[](std::string name)
{
	return getObjectByName(name);
}

void WorldManager::addForce(const IForce& f, IPhysicsNode* o)
{
	_internalForces.add(f.getNewForcePtr(), o);
}

void WorldManager::addForce(IForce* f, IPhysicsNode* o)
{
	_externalForces.add(f, o);
}

void WorldManager::addForce(const IForce& forceToAdd, std::string o)
{
	_internalForces.add(forceToAdd.getNewForcePtr(), getObjectByName(o));
}

void WorldManager::addForce(IForce* forceToAdd, std::string o)
{
	_externalForces.add(forceToAdd, getObjectByName(o));
}

void WorldManager::attachCollisionManager(ICollisionManager* t)
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