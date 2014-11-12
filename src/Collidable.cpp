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
#include "Collidable.h"
using namespace Frost;

Collidable::Collidable()
: _collisionGeometryList(0)
, _collisionGeometryTransforms(0)
, _attachedObject(0)
, _isDirty(true)
{}

Collidable::Collidable(std::shared_ptr<IPhysicsObject> objToAttach)
: _collisionGeometryList(0)
, _collisionGeometryTransforms(0)
, _attachedObject(objToAttach)
{}

Collidable::~Collidable()
{}

void Collidable::attachObject(std::shared_ptr<IPhysicsObject> toAttach)
{
	if (_attachedObject != 0)
	{
		DebugLogger::err("Error - there is already an IPhysicsObject attached to this collidable object!\n");
		throw DuplicateActionException();
	}
	else
	{
		_isDirty = true;
		_attachedObject = toAttach;
	}
}

std::shared_ptr<IPhysicsObject> Collidable::getAttachedObject() const
{
	return _attachedObject;
}

Matrix Collidable::getTransform(int index) const
{
	if (index >= (int)_collisionGeometryTransforms.size()
		|| index < 0)
	{
		std::stringstream ss("");
		ss << "Error: index " << index << " is out of bounds of the collidable array (size " << _collisionGeometryTransforms.size() << ")" << std::endl;
		throw IndexOutOfBoundsException(index);
	}
	else
	{
		if (_attachedObject != 0)
		{
			return _collisionGeometryTransforms[index] * _attachedObject->getTransformMatrix();
		}
		else
		{
			return _collisionGeometryTransforms[index];
		}
	}
}

std::shared_ptr<ICollisionGeometry> Collidable::getCollisionObject(int index) const
{
	if (index < 0
		|| index >= (int)_collisionGeometryList.size())
	{
		throw IndexOutOfBoundsException(index);
	}
	else
	{
		// Update the position of the collision geometry...
		if (_collisionGeometryList[index]->getAttachedObjectPtr() != nullptr)
		{
			auto tf = _collisionGeometryTransforms[index];
			auto obtf = _collisionGeometryList[index]->getAttachedObjectPtr()->getTransformMatrix();
			_collisionGeometryList[index]->setTransformMatrix(tf * obtf);
		}
		return _collisionGeometryList[index];
	}
}

void Collidable::addCollisionObject(std::shared_ptr<ICollisionGeometry> toAdd, const Matrix& t)
{
	if (toAdd == nullptr)
	{
		DebugLogger::err("Error - Collidable::addCollisionObject was provided a null object!\n");
		throw NullObjectException();
	}
	else
	{
		_isDirty = true;
		_collisionGeometryList.push_back(toAdd);
		_collisionGeometryTransforms.push_back(t);
	}
}

void Collidable::removeCollisionObject(int index)
{
	if (index < 0
		|| index >= (int)_collisionGeometryList.size())
	{
		std::stringstream ss("");
		ss << "Error: index " << index << " is out of bounds of the collidable array (size " << _collisionGeometryTransforms.size() << ")" << std::endl;
		DebugLogger::err(ss.str());
		throw IndexOutOfBoundsException(index);
	}
	else
	{
		_isDirty = true;
		_collisionGeometryList.erase(_collisionGeometryList.begin() + index);
		_collisionGeometryTransforms.erase(_collisionGeometryTransforms.begin() + index);
	}
}

bool Collidable::isTouching(std::shared_ptr<Collidable> other)
{
	// First off, update collision geometry transform matrices to the appropriate spots:
	this->updateMatrices();
	other->updateMatrices();

	// Go through our list and the other list, return true if we're touching at all.
	for (auto i = _collisionGeometryList.begin(); i < _collisionGeometryList.end(); ++i)
	{
		for (auto j = other->_collisionGeometryList.begin();
			j < other->_collisionGeometryList.end();
			++j)
		{
			if ((**i).isTouching(**j))
			{
				DebugLogger::debug("Objects are touching\n");
				return true;
			}
		}
	}

	return false;
}

void Collidable::genContacts(std::shared_ptr<Collidable> other, std::vector<std::shared_ptr<IContact>>& o)
{
	// First off, update collision geometry transform matrices to the appropriate spots:
	this->updateMatrices();
	other->updateMatrices();

	for (auto i = _collisionGeometryList.begin(); i < _collisionGeometryList.end(); ++i)
	{
		for (auto j = other->_collisionGeometryList.begin();
			j < other->_collisionGeometryList.end();
			++j)
		{
			unsigned int osize = o.size();
			(**i).genContacts(**j, o);
			if (o.size() > osize)
			{
				std::stringstream ss("");
				ss << (o.size() - osize) << " contacts generated between collidables being tested (";
				ss << (i - _collisionGeometryList.begin()) << ", " << (j - other->_collisionGeometryList.begin()) << ")" << std::endl;
				DebugLogger::debug(ss.str());
			}
		}
	}
}

void Collidable::updateMatrices()
{
	if (_attachedObject == 0)
	{
		for (unsigned int i = 0u; i < _collisionGeometryList.size(); ++i)
		{
			_collisionGeometryList[i]->setTransformMatrix(
				_collisionGeometryTransforms[i]);
		}
	}
	else
	{
		for (unsigned int i = 0u; i < _collisionGeometryList.size(); ++i)
		{
			_collisionGeometryList[i]->setTransformMatrix(
				_collisionGeometryTransforms[i]
				* _attachedObject->getTransformMatrix());
		}
	}
}

bool Collidable::isDirty() const
{
	return _isDirty;
}

void Collidable::clean()
{
	_isDirty = false;
}

int Collidable::getNumObjects() const
{
	return _collisionGeometryList.size();
}