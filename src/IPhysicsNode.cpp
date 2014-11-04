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
#include "IPhysicsNode.h"
using namespace Frost;

IPhysicsNode::IPhysicsNode(std::shared_ptr<IPhysicsObject> ao, std::shared_ptr<Collidable> c, std::string name)
: _obj(ao)
, _collidableData(c)
, _name(name)
{}

IPhysicsNode::IPhysicsNode(const IPhysicsNode& o)
: _obj(o._obj)
, _collidableData(o._collidableData)
, _name(o._name)
{}

IPhysicsNode::~IPhysicsNode()
{}

std::string IPhysicsNode::getName() const
{
	return _name;
}

std::shared_ptr<IPhysicsObject> IPhysicsNode::getPhysicsObject()
{
	return _obj;
}

std::shared_ptr<Collidable> IPhysicsNode::getCollidableData()
{
	return _collidableData;
}

void IPhysicsNode::setName(std::string newName)
{
	if (_name != "")
	{
		throw DuplicateActionException();
	}
	else
	{
		_name = newName;
	}
}