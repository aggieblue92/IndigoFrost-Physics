#include "BVTNode.h"
using namespace Frost;

BVTNode::BVTNode()
	: m_left(0), m_rb(0), m_right(0),
	m_parent(0),
	m_bound()
{}

BVTNode::BVTNode(BVTNode* parent, BoundingSphere* boundingSphere, RigidBody* body)
	: m_left(0), m_right(0), m_parent(parent),
	m_rb(body), m_bound(boundingSphere)
{}

bool BVTNode::isLeaf() {
	return (m_rb != 0);
}

// UNTESTED
// ACTUALLY TESTED AND IT'S TOTAL SHIT.
void BVTNode::Insert(RigidBody* rb, BoundingSphere* bv) {
	// If we are a leaf, create a parent, push down existing leaf,
	//  create new child leaf.
	if(this->isLeaf()) {
		this->m_left = new BVTNode(this, m_bound, m_rb);
		this->m_right = new BVTNode(this, bv, rb);
		this->m_rb = 0; // We're no longer a leaf node - this is the mark.
		// Re-calculate bounding volume
		this->m_bound->getNewBoundingSphere(*this->m_bound, *bv);
	} else {
		// Give it to whoever would grow the least incorporating it.
		if(this->m_left->m_bound->getGrowth(*bv) <
			this->m_right->m_bound->getGrowth(*bv)) {
				this->m_left->Insert(rb, bv);
		} else {
			this->m_right->Insert(rb, bv);
		}
	}
}

unsigned int BVTNode::CoarseCollisionDetect(CoarseContact* o_contacts, unsigned int limit) {
	// Pretty much just compare left and right children.
	
	// If I'm a leaf, then early out.
	if(isLeaf())
		return 0;

	return CoarseCollisionDetect(this->m_left, this->m_right, o_contacts, limit);
}

unsigned int BVTNode::CoarseCollisionDetect(BVTNode* bv1, BVTNode* bv2, CoarseContact* o, unsigned int limit) {
	// Early out - if they are not overlapping, gtfo
	if(!bv1->m_bound->isColliding(*bv2->m_bound))
		return 0;
	if(limit <= 0)
		return 0;

	// If we have two leaf nodes we're looking at, generate contact.
	if(bv1->isLeaf() && bv2->isLeaf()) {
		o->rb1 = bv1->m_rb;
		o->rb2 = bv2->m_rb;
		return 1;
	}

	// We have more to go? Recurse!
	// If leaf/branch, recurse into branch.
	// If two branches, recurse first into larger.
	//  (This is okay, because the recursion will eventually hit everything still)
	if(bv2->isLeaf() ||
		(!bv1->isLeaf() && bv1->m_bound->getVolume() >= bv2->m_bound->getVolume())) {

			// Recurse onto self, keeping track of contacts generated.
			unsigned int genCount = 
				CoarseCollisionDetect(bv1->m_left, bv2, o, limit);

			// If we have enough remaining slots, do other side too.
			if(limit > genCount) {
				return
					CoarseCollisionDetect(bv1->m_right, bv2, o+genCount, limit - genCount) +
						genCount;
			} else {
				return genCount;
			}
	} else {
		unsigned int genCount =
			CoarseCollisionDetect(bv1, bv2->m_left, o, limit);

		if(limit > genCount) {
			return
				CoarseCollisionDetect(bv1, bv2->m_right, o+genCount, limit - genCount) +
					genCount;
		} else {
			return genCount;
		}
	}
}

void BVTNode::Release() {
	
	if(this->m_parent != 0) {
		// Find sibling, yep.
		BVTNode* sibling;
		if(this->m_parent->m_left == this)
			sibling = m_parent->m_right;
		else
			sibling = m_parent->m_left;

		// Write data to our parent...
		m_parent->m_bound = sibling->m_bound;
		m_parent->m_rb = sibling->m_rb;
		m_parent->m_left = sibling->m_left;
		m_parent->m_right = sibling->m_right;

		// Delete the sibling Note- do not have to release it first.
		delete sibling;

		// Recalculate bounding sphere?
	}

	// Delete our children
	if(m_left != 0) {
		m_left->Release();
		delete m_left;
	}
	if(m_right != 0) {
		m_right->Release();
		delete m_right;
	}
}