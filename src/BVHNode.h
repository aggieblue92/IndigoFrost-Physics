#ifndef FROST_PHYSICS_NODE_BVH_H
#define FROST_PHYSICS_NODE_BVH_H

/////////////////////////////////////////
// BVHNode: Node in a BVHTree.
/////////////////////////////////////////

#include "IPhysicsNode.h"
#include "Collidable.h"
#include "BoundingSphere.h"

namespace Frost
{
	class BVHNode : public IPhysicsNode
	{
	public:
		BVHNode(Collidable* attachedCollidable, std::string name ="");
		~BVHNode();

		BVHNode* insert(Collidable* toAttach, std::string name = "");
		void remove(std::string toRemove);
		void remove(Collidable* toRemove);

		void updateBoundingVolumes();

		BVHNode* getDeepestElement();

		int getDepth() const;

		BVHNode* getLeftChild() const;
		BVHNode* getRightChild() const;

		BoundingSphere& getVolume() const;

		bool isLeaf() const;

	private:
		BVHNode* _left;
		BVHNode* _right;
		BoundingSphere* _volume;

		void getDeepestElement(BVHNode*& o_deepest, int& depth, int myDepth = 0);
	};
}

#endif