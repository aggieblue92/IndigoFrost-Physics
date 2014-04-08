#ifndef INDIGO_FROST_BOUNDING_VOLUME_TREE_NODE_H
#define INDIGO_FROST_BOUNDING_VOLUME_TREE_NODE_H

/*****************************************************\

		BVTNode - Node of a tree for a Bounding Volume
	Hierarchy. Used to perform coarse collision detection.

	Contains CoarseContact, a struct that contains the
result of a coarse collision detection returning a
positive. This will be passed to fine collision detection
for further analysis.

Members:
   m_left    Left child of binary tree
   m_right   Right child of binary tree
   m_boundingVolume   Bounding volume encompassed
      by the node in question.
   m_rb      Rigid body IF leaf. If not leaf, this is
      NULL always. This is how we know we're at a leaf.


Insertion:
  If this->object is null, insert into this (root case)
  If this is a leaf, then:
    leftchild is this
	rightchild is toInsert
	this is parent of the two
  If this is a branch:
    Insert recursively onto object which would be smaller upon
	 addition of the new object down that way.
\*****************************************************/

#include "BoundingSphere.h"
#include "RigidBody.h"
#include <vector>

namespace Frost {

	struct CoarseContact {
		RigidBody* rb1;
		RigidBody* rb2;
		CoarseContact() : rb1(0), rb2(0) {}
		CoarseContact(RigidBody* r1, RigidBody* r2) : rb1(r1), rb2(r2) {}
	};

	class BVTNode {
	private:
		BVTNode* m_left;
		BVTNode* m_right;
		RigidBody* m_rb; // NULL except for leaf nodes
		BoundingSphere* m_volume; // NULL for empty tree.

		// Returns whether or not this node is a leaf node
		bool isLeaf();

		// Used to find the deepest element in the tree.
		void getDeepestElement(BVTNode*& deepest, int& depth, int myDepth=0);

	protected:
		// Perform coarse collision detection on the two nodes, on left and right.
		//  Static function - this is not actually attached to the tree.
		static unsigned int CoarseCollision(BVTNode* checkLeft, BVTNode* checkRight, std::vector<CoarseContact>& o_contacts, unsigned int limit);

	public:
		BVTNode();
		~BVTNode();

		// Inserts a new rigid body into the tree.
		void Insert(RigidBody* newObject);
		
		// Removes a rigid body from the tree.
		void Remove(RigidBody* toRemove);

		// Updates the bounding volumes on the tree.
		void Update();

		// Returns the deepest element in the tree. Used for re-balancing.
		BVTNode* getDeepestElement();

		int getDepth() const;

		// Performs coarse collision detection, filling this list. This is called
		//  externally, but pretty much only calls private version.
		unsigned int CoarseCollision(std::vector<CoarseContact>& o_contacts, unsigned int limit);

		// Create a new tree based on the old one. Re-balancing.
		//  This function clears old memory.
		// CURRENTLY PART OF BROKEN CODE - DO NOT USE YET.
		static BVTNode* createRebalancedTree(const BVTNode* oldTree);

		// BEGIN DEBUG FUNCTIONS
		void GetNodesAtDepth(std::vector<BoundingSphere*>& o_nodes, int level, int myLevel = 0) const;
		void GetBodiesAtDepth(std::vector<RigidBody*>& o_bodies, int level, int myLevel = 0) const;
		// END DEBUG FUNCTIONS
	};
}

#endif
