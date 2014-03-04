#include "BVTNode.h"
using namespace Frost;

BVTNode::BVTNode()
	: m_left(0), m_rb(0), m_right(0),
	m_boundingVolume()
{}

bool BVTNode::isLeaf() {
	return (m_rb == 0);
}

unsigned int BVTNode::CoarseCollisionDetect(CoarseContact* o_contacts, unsigned int limit) {
	// Pretty much just compare left and right children.
	return CoarseCollisionDetect(this->m_left, this->m_right, o_contacts, limit);
}

unsigned int BVTNode::CoarseCollisionDetect(BVTNode* bv1, BVTNode* bv2, CoarseContact* o, unsigned int limit) {
	// Early out - if they are not overlapping, gtfo
	if(!bv1->m_boundingVolume.isColliding(bv2->m_boundingVolume))
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
	if(bv1->isLeaf() ||
		(!bv2->isLeaf() && bv2->m_boundingVolume.getVolume() >= bv1->m_boundingVolume.getVolume())) {

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