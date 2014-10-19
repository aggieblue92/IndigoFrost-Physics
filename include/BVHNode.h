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
#ifndef FROST_PHYSICS_NODE_BVH_H
#define FROST_PHYSICS_NODE_BVH_H

/////////////////////////////////////////
// BVHNode: Node in a BVHTree.
/////////////////////////////////////////

#include "IPhysicsNode.h"
#include "Collidable.h"
#include "BoundingSphere.h"
#include <memory>

namespace Frost
{
	class BVHNode : public IPhysicsNode
	{
	public:
		BVHNode(std::shared_ptr<Collidable> attachedCollidable, std::string name ="");
		~BVHNode();

		std::shared_ptr<BVHNode> insert(std::shared_ptr<Collidable> toAttach, std::string name = "");
		void remove(std::string toRemove);
		void remove(std::shared_ptr<Collidable> toRemove);

		void updateBoundingVolumes();

		std::shared_ptr<BVHNode> getDeepestElement();

		int getDepth() const;

		std::shared_ptr<BVHNode> getLeftChild() const;
		std::shared_ptr<BVHNode> getRightChild() const;

		BoundingSphere& getVolume() const;

		bool isLeaf() const;

	private:
		std::shared_ptr<BVHNode> _left;
		std::shared_ptr<BVHNode> _right;
		std::shared_ptr<BoundingSphere> _volume;

		void getDeepestElement(std::shared_ptr<BVHNode>& o_deepest, int& depth, int myDepth = 0);
	};
}

#endif