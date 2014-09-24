#include "Collidable.h"
using namespace Frost;

Collidable::Collidable()
: _collisionGeometryList(0)
, _collisionGeometryTransforms(0)
, _attachedObject(0)
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
			return _collisionGeometryTransforms[index] * _attachedObject->GetTransformMatrix();
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
				* _attachedObject->GetTransformMatrix());
		}
	}
}

int Collidable::getNumObjects() const
{
	return _collisionGeometryList.size();
}