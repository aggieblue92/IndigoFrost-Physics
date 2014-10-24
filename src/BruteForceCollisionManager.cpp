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

#include "BruteForceCollisionManager.h"
using namespace Frost;

BruteForceCollisionManager::BruteForceCollisionManager()
: _objList(0)
{}

void BruteForceCollisionManager::addPhysicsNode(std::shared_ptr<IPhysicsNode> t)
{
	_objList.push_back(t);
}

void BruteForceCollisionManager::removePhysicsNode(std::shared_ptr<IPhysicsNode> r)
{
	for (unsigned int i = 0u; i < _objList.size(); ++i)
	{
		if (_objList[i] == r)
		{
			_objList.erase(_objList.begin() + i);
			return;
		}
	}
}

void BruteForceCollisionManager::removePhysicsNode(std::string name)
{
	for (unsigned int i = 0u; i < _objList.size(); ++i)
	{
		if (_objList[i]->getName() == name)
		{
			_objList.erase(_objList.begin() + i);
			return;
		}
	}
}

void BruteForceCollisionManager::genContacts(std::vector<std::shared_ptr<IContact>>& o)
{
#if defined(_DEBUG)
	std::vector<std::shared_ptr<IContact>> test(0);
	for (unsigned int i = 0u; i < _objList.size(); ++i)
	{
		for (unsigned int j = i + 1u; j < _objList.size(); ++j)
		{
			_objList[i]->getCollidableData()->genContacts(_objList[j]->getCollidableData(), test);
		}
	}
	int breakOn = test.size();
#endif

	for (unsigned int i = 0u; i < _objList.size(); ++i)
	{
		for (unsigned int j = i + 1u; j < _objList.size(); ++j)
		{
			_objList[i]->getCollidableData()->genContacts(_objList[j]->getCollidableData(), o);
		}
	}
}

void BruteForceCollisionManager::update(float dt)
{
	// Don't have to do anything here.
}