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

WorldManager::~WorldManager()
{
	while (_masterContactList.size() > 0)
	{
		delete *_masterContactList.begin();
		*_masterContactList.begin() = 0;
		_masterContactList.erase(_masterContactList.begin());
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
		(*i)->getPhysicsObject()->Update(timeElapsed);
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

IPhysicsNode* WorldManager::addObject(IPhysicsObject* objectToAdd, std::string name)
{
	// Make sure the name does not exist...
	for (auto i = _allManagedObjects.begin(); i < _allManagedObjects.end(); ++i)
	{
		if ((*i)->getName() == name)
		{
			throw DuplicateActionException();
		}
	}

	IPhysicsNode* nodeToAdd = new IPhysicsNode(objectToAdd, name);
	_collisionManager->addPhysicsNode(nodeToAdd);
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