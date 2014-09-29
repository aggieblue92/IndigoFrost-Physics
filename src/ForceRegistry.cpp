#include "ForceRegistry.h"
using namespace Frost;

ForceEntry::ForceEntry(IForce* f, IPhysicsNode* n)
: _force(f), _obj(n) {}

ForceRegistry::ForceRegistry()
: _forcesList()
{}

void ForceRegistry::Add(IForce* force, IPhysicsNode* physicsNode)
{
	_forcesList.push_back(ForceEntry(force, physicsNode));
}

bool ForceRegistry::Remove(IForce* force, IPhysicsNode* pn)
{
	bool toReturn = false;
	for (auto i = _forcesList.begin(); i < _forcesList.end(); ++i)
	{
		if ((i->_force == force || force == 0)
			&& (i->_obj == pn || pn == 0))
		{
			_forcesList.erase(i);
			toReturn = true;
		}
	}
	return toReturn;
}

unsigned int ForceRegistry::size() const
{
	return _forcesList.size();
}

ForceEntry& ForceRegistry::operator[](unsigned int i)
{
	return _forcesList[i];
}

void ForceRegistry::ClearRegistry()
{
	while (_forcesList.size() > 0)
	{
		_forcesList.erase(_forcesList.begin());
	}
}

void ForceRegistry::UpdateForces(float timeElapsed)
{
	for (auto i = _forcesList.begin(); i < _forcesList.end(); ++i)
	{
		i->_force->ApplyForce(i->_obj->getPhysicsObject(), timeElapsed);
	}
}