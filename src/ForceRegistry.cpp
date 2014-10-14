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
#include "ForceRegistry.h"
using namespace Frost;

ForceEntry::ForceEntry(IForce* f, IPhysicsNode* n)
: _force(f), _obj(n) {}

ForceRegistry::ForceRegistry()
: _forcesList()
{}

void ForceRegistry::add(IForce* force, IPhysicsNode* physicsNode)
{
	_forcesList.push_back(ForceEntry(force, physicsNode));
}

bool ForceRegistry::remove(IForce* force, IPhysicsNode* pn)
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

void ForceRegistry::clearRegistry()
{
	while (_forcesList.size() > 0)
	{
		_forcesList.erase(_forcesList.begin());
	}
}

void ForceRegistry::updateForces(float timeElapsed)
{
	for (auto i = _forcesList.begin(); i < _forcesList.end(); ++i)
	{
		i->_force->applyForce(i->_obj->getPhysicsObject(), timeElapsed);
	}
}