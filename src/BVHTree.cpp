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
#include "BVHTree.h"
using namespace Frost;

BVHTree::BVHTree()
: _root(0)
{
	_root = std::make_shared<BVHNode>(std::shared_ptr<Collidable>(0), "");
}

BVHTree::~BVHTree()
{}

void BVHTree::addPhysicsNode(std::shared_ptr<IPhysicsNode> t)
{
	if (t == 0)
	{
		throw NullObjectException();
	}
	else if (t->getCollidableData() == 0)
	{
		throw NullObjectException();
	}
	else
	{
		_root->insert(t->getCollidableData(), t->getName());
	}
}

void BVHTree::removePhysicsNode(std::shared_ptr<IPhysicsNode> toRemove)
{
	if (toRemove == 0) return;
	else if (toRemove->getName() != "")
	{
		_root->remove(toRemove->getName());
	}
	else if (toRemove->getCollidableData() != 0)
	{
		_root->remove(toRemove->getCollidableData());
	}
}

void BVHTree::removePhysicsNode(std::string toRemove)
{
	if (toRemove != "")
	{
		_root->remove(toRemove);
	}
}

void BVHTree::genContacts(std::vector<std::shared_ptr<IContact>>& o_contactList)
{
	// If the left and right side aren't touching, just go
	//  down the left and right sides, giving each 1/2 of the contact limit
	if (_root == 0) return;
	else if (_root->getLeftChild() == 0 || _root->getRightChild() == 0) return;
	else genContacts(o_contactList, _root->getLeftChild(), _root->getRightChild());
}

void BVHTree::genContacts(std::vector<std::shared_ptr<IContact>>& o_contactList, std::shared_ptr<BVHNode> l, std::shared_ptr<BVHNode> r)
{
	if (l == 0 || r == 0) return;

	// If the left and right side aren't touching, just go down the left and right sides,
	//  giving each 1/2 of the contact limit.
	if (!l->getVolume().isColliding(r->getVolume()))
	{
		genContacts(o_contactList, l->getLeftChild(), l->getRightChild());
		genContacts(o_contactList, r->getLeftChild(), r->getRightChild());
	}

	// If the left side is a leaf node (rigid body)...
	else if (l->isLeaf())
	{
		// If right is also a leaf, flag potential contact for the two.
		if (r->isLeaf())
		{
			l->getCollidableData()->genContacts(r->getCollidableData(), o_contactList);
		}

		// If right is a branch, recurse down right side.
		else
		{
			genContacts(o_contactList, l, r->getLeftChild());
			genContacts(o_contactList, l, r->getRightChild());
			genContacts(o_contactList, r->getLeftChild(), r->getRightChild());
		}
	}

	// If the left side is a branch (non-leaf)...
	else
	{
		// Regardless of right node status, recurse down both right children
		genContacts(o_contactList, l->getLeftChild(), l->getRightChild());

		// If the right side is a leaf, recurse onto left
		if (r->isLeaf())
		{
			genContacts(o_contactList, l->getLeftChild(), r);
			genContacts(o_contactList, l->getRightChild(), r);
		}

		// If the right side is also a branch, God help us all.
		else
		{
			genContacts(o_contactList, l->getLeftChild(), r->getLeftChild());
			genContacts(o_contactList, l->getLeftChild(), r->getRightChild());
			genContacts(o_contactList, l->getRightChild(), r->getLeftChild());
			genContacts(o_contactList, l->getRightChild(), r->getRightChild());
			genContacts(o_contactList, r->getLeftChild(), r->getRightChild());
		}
	}
}

void BVHTree::update(float dt)
{
	// You should do this on its own thread... or something
	_root->updateBoundingVolumes();
}