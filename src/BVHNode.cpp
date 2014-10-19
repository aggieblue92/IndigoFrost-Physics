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
#include "BVHNode.h"
using namespace Frost;

BVHNode::BVHNode(std::shared_ptr<Collidable> ac, std::string name)
: IPhysicsNode((ac.get() == 0) ? std::shared_ptr<IPhysicsObject>(0) : ac->getAttachedObject(), ac, name)
, _left(0)
, _right(0)
, _volume(0)
{
	if (ac != 0)
	{
		_volume = std::make_shared<BoundingSphere>(*ac);
	}
}

BVHNode::~BVHNode()
{}

std::shared_ptr<BVHNode> BVHNode::insert(std::shared_ptr<Collidable> toAttach, std::string name)
{
	// If tree is empty, make this the new root
	if (_volume == 0)
	{
		if (toAttach == 0)
			throw NullObjectException();

		if (_left != 0 || _right != 0 || _obj != 0)
			throw InvalidOperationException();

		_obj = toAttach->getAttachedObject();
		_collidableData = toAttach;
		_volume = std::make_shared<BoundingSphere>(*toAttach);

		_name = name;
	}

	// If this is a leaf, make it a parent, where the
	//  children are this and the newObject node.
	else if (this->isLeaf())
	{
		_left = std::make_shared<BVHNode>(_collidableData, _name);
		_right = std::make_shared<BVHNode>(toAttach, name);
		_collidableData = 0;
		_obj = 0;
	}

	else
	{
		// Look to the left!
		BoundingSphere newSphere(*toAttach);
		if (_left->_volume->getNewRadius(newSphere) < _right->_volume->getNewRadius(newSphere))
		{
			_left->insert(toAttach, name);
		}
		else
		{
			_right->insert(toAttach, name);
		}

		// After the recursion, update our own bounding volume.
		*_volume = BoundingSphere::getNewBoundingSphere(*_left->_volume, *_right->_volume);
	}
	return 0;
}

void BVHNode::remove(std::string toRemove)
{
	throw NotImplementedException();
}

void BVHNode::remove(std::shared_ptr<Collidable> toRemove)
{
	throw NotImplementedException();
}

void BVHNode::updateBoundingVolumes()
{
	if (_left != 0)
	{
		_left->updateBoundingVolumes();
	}

	if (_right != 0)
	{
		_right->updateBoundingVolumes();
	}

	if (_volume != 0)
	{
		if (isLeaf())
		{
			*_volume = BoundingSphere(*_collidableData);
		}
		else
		{
			*_volume = BoundingSphere::getNewBoundingSphere(*_left->_volume, *_right->_volume);
		}
	}
}

std::shared_ptr<BVHNode> BVHNode::getDeepestElement()
{
	std::shared_ptr<BVHNode> toReturn(nullptr);
	int deepest = -1;
	getDeepestElement(toReturn, deepest, 0);
	return toReturn;
}

int BVHNode::getDepth() const
{
	throw NotImplementedException();
}

std::shared_ptr<BVHNode> BVHNode::getLeftChild() const
{
	return _left;
}

std::shared_ptr<BVHNode> BVHNode::getRightChild() const
{
	return _right;
}

BoundingSphere& BVHNode::getVolume() const
{
	return *_volume;
}

bool BVHNode::isLeaf() const
{
	return _collidableData != 0;
}

void BVHNode::getDeepestElement(std::shared_ptr<BVHNode>& o_deepest, int& depth, int myDepth)
{
	if (myDepth > depth)
	{
		o_deepest = std::shared_ptr<BVHNode>(this);
		depth = myDepth;
	}

	if (_left != 0)
	{
		_left->getDeepestElement(o_deepest, depth, myDepth + 1);
	}
	if (_right != 0)
	{
		_right->getDeepestElement(o_deepest, depth, myDepth + 1);
	}
}