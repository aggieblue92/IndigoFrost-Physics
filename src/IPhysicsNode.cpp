#include "IPhysicsNode.h"
using namespace Frost;

IPhysicsNode::IPhysicsNode(IPhysicsObject* ao, Collidable* c, std::string name)
: _obj(ao)
, _name(name)
, _collidableData(c)
{}

IPhysicsNode::~IPhysicsNode()
{}

IPhysicsNode::operator Frost::IPhysicsObject*()
{
	return _obj;
}

std::string IPhysicsNode::getName() const
{
	return _name;
}

IPhysicsObject* IPhysicsNode::getPhysicsObject()
{
	return _obj;
}

Collidable* IPhysicsNode::getCollidableData()
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