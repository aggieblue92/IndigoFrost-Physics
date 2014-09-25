#include "BVHNode.h"
using namespace Frost;

BVHNode::BVHNode(Collidable* ac, std::string name)
: IPhysicsNode((ac == 0) ? 0 : ac->getAttachedObject(), ac, name)
, _left(0)
, _right(0)
, _volume(0)
{
	if (ac != 0)
	{
		_volume = new BoundingSphere(*ac);
	}
}

BVHNode::~BVHNode()
{
	if (_left != 0)
	{
		delete _left;
		_left = 0;
	}
	if (_right != 0)
	{
		delete _right;
		_right = 0;
	}
	
	delete _volume; 
	_volume = 0;
	IPhysicsNode::~IPhysicsNode();
}

BVHNode* BVHNode::insert(Collidable* toAttach, std::string name)
{
	// If tree is empty, make this the new root
	if (_collidableData == 0)
	{
		if (toAttach == 0)
			throw NullObjectException();

		if (_left != 0 || _right != 0 || _obj != 0)
			throw InvalidOperationException();

		_obj = toAttach->getAttachedObject();
		_collidableData = toAttach;
		
		_name = name;
	}

	// If this is a leaf, make it a parent, where the
	//  children are this and the newObject node.
	else if (this->isLeaf())
	{
		_left = new BVHNode(_collidableData, this->_name);
		_right = new BVHNode(toAttach, name);
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

void BVHNode::remove(Collidable* toRemove)
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

BVHNode* BVHNode::getDeepestElement()
{
	BVHNode* toReturn = 0;
	int deepest = -1;
	getDeepestElement(toReturn, deepest, 0);
	return toReturn;
}

int BVHNode::getDepth() const
{
	throw NotImplementedException();
}

BVHNode* BVHNode::getLeftChild() const
{
	return _left;
}

BVHNode* BVHNode::getRightChild() const
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

void BVHNode::getDeepestElement(BVHNode*& o_deepest, int& depth, int myDepth)
{
	if (myDepth > depth)
	{
		o_deepest = this;
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