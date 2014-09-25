#include "WorldManager.h"
#include <thread>
using namespace Frost;

int WorldManager::nInstances = 0;

WorldManager::WorldManager()
: _masterContactList(0)
, _forces()
, _collisionManager(0)
{
	if (nInstances > 0)
		throw DuplicateActionException();

	nInstances++;
}

WorldManager::WorldManager(FROST_COLLISION_MANAGER cmt)
: _masterContactList(0)
, _forces()
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
	nInstances--;
}

void WorldManager::update(float timeElapsed)
{
	if (_collisionManager == 0)
	{
		throw NotImplementedException();
	}

	_collisionManager->update(timeElapsed);
	_forces.UpdateForces(timeElapsed);

	for (auto i = _allManagedObjects.begin(); i < _allManagedObjects.end(); ++i)
	{
		(*i)->getPhysicsObject()->update(timeElapsed);
	}

	_collisionManager->genContacts(_masterContactList);

	// TODO: In the future, you should always have a thread running that updates
	//  all this, and then just query it at the frame.
	// Right now, just handle all contacts.
	for (auto i = _masterContactList.begin(); i < _masterContactList.end(); ++i)
	{
		(*i)->Resolve();
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

void WorldManager::addForce(IForce* f, IPhysicsNode* o)
{
	_forces.Add(f, o);
}

void WorldManager::addForce(IForce* f, std::string o)
{
	_forces.Add(f, getObjectByName(o));
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