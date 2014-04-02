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

\*****************************************************/

#include "BoundingSphere.h"
#include "RigidBody.h"

namespace Frost {

	struct CoarseContact {
		RigidBody* rb1;
		RigidBody* rb2;
		CoarseContact() : rb1(0), rb2(0) {}
	};

	class BVTNode {
	public:
		BVTNode();
		BVTNode(BVTNode* parent, BoundingSphere* boundingSphere, RigidBody* body);

		bool isLeaf();
		void Insert(RigidBody* body, BoundingSphere* boundingVolume);
		void Release();

		unsigned int CoarseCollisionDetect(CoarseContact* o_contacts, unsigned int limit);

	private:
		BVTNode* m_parent;
		BVTNode* m_left;
		BVTNode* m_right;

	protected:
		// TODO: Update to include boxes and such.
		BoundingSphere* m_bound;
		RigidBody* m_rb;

		unsigned int CoarseCollisionDetect(BVTNode* bv1, BVTNode* bv2, CoarseContact* o_coarseContacts, unsigned int limit);
	};
}

#endif
