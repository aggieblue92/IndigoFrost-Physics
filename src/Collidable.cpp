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

Collidable::Collidable(IPhysicsObject* objToAttach)
: _collisionGeometryList(0)
, _collisionGeometryTransforms(0)
, _attachedObject(objToAttach)
{}

Collidable::~Collidable()
{
	for (unsigned int i = 0u; i < _collisionGeometryList.size(); ++i)
	{
		delete _collisionGeometryList[i];
		_collisionGeometryList[i] = 0;
	}
	_collisionGeometryList.resize(0);
}

void Collidable::attachObject(IPhysicsObject* toAttach)
{
	if (_attachedObject != 0)
	{
		throw DuplicateActionException();
	}
	else
	{
		_isDirty = true;
		_attachedObject = toAttach;
	}
}

IPhysicsObject* Collidable::getAttachedObject() const
{
	return _attachedObject;
}

Matrix Collidable::getTransform(int index) const
{
	if (index >= (int)_collisionGeometryTransforms.size()
		|| index < 0)
	{
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

ICollisionGeometry* Collidable::getCollisionObject(int index) const
{
	if (index < 0
		|| index >= (int)_collisionGeometryList.size())
	{
		throw IndexOutOfBoundsException(index);
	}
	else
	{
		return _collisionGeometryList[index];
	}
}

void Collidable::addCollisionObject(ICollisionGeometry* toAdd, const Matrix& t)
{
	if (toAdd == 0)
	{
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
		throw IndexOutOfBoundsException(index);
	}
	else
	{
		_isDirty = true;
		delete _collisionGeometryList[index];
		_collisionGeometryList[index] = 0;
		_collisionGeometryList.erase(_collisionGeometryList.begin() + index);
		_collisionGeometryTransforms.erase(_collisionGeometryTransforms.begin() + index);
	}
}

bool Collidable::isTouching(Collidable* other)
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
			if ((**i).isTouching(*j))
			{
				return true;
			}
		}
	}

	return false;
}

void Collidable::genContacts(Collidable* other, std::vector<IContact*>& o)
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
			(**i).genContacts(*j, o);
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