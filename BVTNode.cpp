#include "BVTNode.h"
using namespace Frost;

BVTNode::BVTNode()
: m_left(0), m_right(0), m_rb(0), m_volume(0)
{}

BVTNode::~BVTNode() {
	// Not sure if I should do this this way...
	if (m_left != 0) {
		delete m_left;
		m_left = 0;
	}
	if (m_right != 0) {
		delete m_right;
		m_right = 0;
	}
	if (m_volume != 0) {
		delete m_volume;
		m_volume = 0;
	}
}

bool BVTNode::isLeaf() {
	// Leaf if and only if rigid body is non-null
	return (m_rb != 0);
}

// Used to hfind deepest elemeent in a tree.
void BVTNode::getDeepestElement(BVTNode*& deepest, int& depth, int myDepth) {
	if (myDepth > depth) {
		deepest = this;
		depth = myDepth;
	}
	if (this->m_left != 0) {
		this->m_left->getDeepestElement(deepest, depth, myDepth + 1);
	}
	if (this->m_right != 0) {
		this->m_right->getDeepestElement(deepest, depth, myDepth + 1);
	}
}
// THE PROBLEM SEEMS TO BE HERE - it's not checking properly both sides of the tree together. Must consider left->left and left->right together
//  as well.
unsigned int BVTNode::CoarseCollision(BVTNode* checkLeft, BVTNode* checkRight, std::vector<CoarseContact>& o_contacts, unsigned int limit) {
	// If the left and right side aren't touching, just go down the left and right sides, giving each
	//  1/2 of the contact limit (CHANGE THIS)
	if (!checkLeft->m_volume->isColliding(*checkRight->m_volume)) {
		unsigned int nGenerated = 0;
		if(!checkLeft->isLeaf()) {
			nGenerated += BVTNode::CoarseCollision(checkLeft->m_left, checkLeft->m_right, o_contacts, limit / 2);
		}
		if(!checkRight->isLeaf()) {
			nGenerated += BVTNode::CoarseCollision(checkRight->m_left, checkRight->m_right, o_contacts, limit / 2);
		}
	}

	// If our left side is a leaf node (rigid body)...
	if (checkLeft->isLeaf()) {
		// If right is also a leaf, then flag potential contact for the two.
		if (checkRight->isLeaf()) {
			CoarseContact* gen = new CoarseContact;
			o_contacts.push_back(CoarseContact(checkLeft->m_rb, checkRight->m_rb));
			return 1;
		}

		// If right is a branch, recurse down onto right side.
		else {
			unsigned int lSide = 0;
			// Also recurse down onto both right children together.
			if (limit - lSide > 0) {
				lSide +=
					BVTNode::CoarseCollision(checkRight->m_left, checkRight->m_right, o_contacts, limit - lSide);
			}
			if (limit - lSide > 0) {
				lSide += BVTNode::CoarseCollision(checkLeft, checkRight->m_right, o_contacts, limit - lSide);
			}
			if (limit - lSide > 0) {
				lSide += BVTNode::CoarseCollision(checkLeft, checkRight->m_left, o_contacts, limit - lSide);
			}
			return lSide;
		}
	}

	// If our left side is a branch node (non-leaf)...
	else {
		// Regardless of right node status, recurse down both children on left.
		unsigned int rSide = 0;
		rSide += BVTNode::CoarseCollision(checkLeft->m_left, checkLeft->m_right, o_contacts, limit);

		// If the right side is a leaf, recurse onto left
		if (checkRight->isLeaf()) {
			if (limit - rSide > 0) {
				rSide += BVTNode::CoarseCollision(checkLeft->m_right, checkRight, o_contacts, limit - rSide);
			}
			if (limit - rSide > 0) {
				return rSide +
					BVTNode::CoarseCollision(checkLeft->m_left, checkRight, o_contacts, limit - rSide);
			}
			else {
				return rSide;
			}
		}

		// If right side is also a branch, God help us all.
		else {
			// Recurse in all four directions.
			//  LL, LR, RL, RR
			unsigned int nGenerated = rSide;
			if(limit - nGenerated > 0) {
				nGenerated += BVTNode::CoarseCollision(checkLeft->m_left, checkRight->m_left, o_contacts, limit - rSide);
			}
			if (limit - nGenerated > 0) {
				nGenerated +=
					BVTNode::CoarseCollision(checkLeft->m_right, checkRight->m_left, o_contacts, limit - nGenerated);
			}
			if (limit - nGenerated > 0) {
				nGenerated +=
					BVTNode::CoarseCollision(checkLeft->m_left, checkRight->m_right, o_contacts, limit - nGenerated);
			}
			if (limit - nGenerated > 0) {
				nGenerated +=
					BVTNode::CoarseCollision(checkLeft->m_right, checkRight->m_right, o_contacts, limit - nGenerated);
			}

			return nGenerated;
		}
	}

	// If the left side is a leaf, and the right side is
	return 4;
}

void BVTNode::Insert(RigidBody* newObject) {
	// If the tree is empty, make this the new root
	if (m_volume == 0) {
		m_left = 0;
		m_right = 0;
		m_rb = newObject;
		m_volume = new BoundingSphere;
		m_volume->fillFromRigidBody(*newObject);
		return;
	}

	// If this is a leaf, make it a parent, where the
	//  children are this and the newObject node.
	if (this->isLeaf()) {
		// Left child is this...
		this->m_left = new BVTNode;
		this->m_left->m_rb = this->m_rb;
		this->m_left->m_volume = this->m_volume;

		// Right child is our inserted object...
		this->m_right = new BVTNode;
		this->m_right->m_rb = newObject;
		// Does this next line even work?
		this->m_right->m_volume = new BoundingSphere;
		this->m_right->m_volume->fillFromRigidBody(*newObject);

		// Finally, this is now a parent.
		this->m_rb = 0;
		this->m_volume = new BoundingSphere();
		*this->m_volume = BoundingSphere::getNewBoundingSphere(*this->m_left->m_volume, *this->m_right->m_volume);

		// Get out!
		return;
	}

	// We're at a branch? See which direction leads in the smallest object.
	//  If left, recurse left. If right, recurse right.
	
	// Create bounding volume...
	BoundingSphere newSphere;
	newSphere.fillFromRigidBody(*newObject);

	// Look to left and right.
	if (m_left->m_volume->getNewRadius(newSphere) < m_right->m_volume->getNewRadius(newSphere)) {
		m_left->Insert(newObject);
	}
	else {
		m_right->Insert(newObject);
	}

	// We're done with the recursion, so update our bounding volume.
	//  We should be at a branch on this level.
	*this->m_volume = BoundingSphere::getNewBoundingSphere(*this->m_left->m_volume, *this->m_right->m_volume);
}

void BVTNode::Remove(RigidBody* toRemove) {
	// If the node to the left has this rigid body, replace
	//  self with node to the right.
	if (m_left != 0) {
		if (m_left->m_rb == toRemove) {
			if (m_right != 0) {
				m_rb = m_right->m_rb;
				m_volume = m_right->m_volume;
			}
		}
	}
}

void BVTNode::Update() {
	// First off, update left and right children:
	if (m_left != 0)
		m_left->Update();
	if (m_right != 0)
		m_right->Update();

	// This goes through everything, from bottom to top,
	//  and updates all the bounding volumes.
	if (this->isLeaf()) {
		// We are at a leaf, re-calcualte based on
		//  rigid body information.
		m_volume->fillFromRigidBody(*m_rb);
	}

	else {
		// Re-calculate based on children nodes.
		*m_volume = BoundingSphere::getNewBoundingSphere(*m_left->m_volume, *m_right->m_volume);
	}
}

BVTNode* BVTNode::getDeepestElement() {
	BVTNode* toReturn = 0;
	int deepest = -1;
	this->getDeepestElement(toReturn, deepest, 0);
	return toReturn;
}

unsigned int BVTNode::CoarseCollision(std::vector<CoarseContact>& o_list, unsigned int limit) {
	return BVTNode::CoarseCollision(m_left, m_right, o_list, limit);
}

void BVTNode::GetNodesAtDepth(std::vector<BoundingSphere*>& o_nodes, int level, int myLevel) const {
	if(myLevel == level) {
		o_nodes.push_back(m_volume);
	}
	else {
		if(m_left != 0) {
			m_left->GetNodesAtDepth(o_nodes, level, myLevel + 1);
		}
		if(m_right != 0) {
			m_right->GetNodesAtDepth(o_nodes, level, myLevel + 1);
		}
	}
}

////////////////===========================/////////////////////
// Everything AFTER THIS POINT is potentially broken.
int BVTNode::getDepth() const {
	if(m_rb != 0)
		return 1;
	else {
		int lSide = m_left->getDepth();
		int rSide = m_right->getDepth();
		if (lSide > rSide)
			return 1 + lSide;
		else
			return 1 + rSide;
	}
}

void BVTNode::GetBodiesAtDepth(std::vector<RigidBody*>& o_bodies, int level, int myLevel) const {
	if(myLevel == level) {
		o_bodies.push_back(m_rb);
	}
	else {
		if(m_left != 0) {
			m_left->GetBodiesAtDepth(o_bodies, level, myLevel + 1);
		}
		if(m_right != 0) {
			m_right->GetBodiesAtDepth(o_bodies, level, myLevel + 1);
		}
	}
}

// Basically, create a new tree from the old one.
BVTNode* BVTNode::createRebalancedTree(const BVTNode* oldTree) {
	// First off, build a new tree.
	BVTNode* toReturn = new BVTNode;

	// Get depth of the tree
	int depth = oldTree->getDepth();

	// Start at that depth, go to zero, insert nodes.
	for(int i = depth; i >= 0; i--) {
		std::vector<RigidBody*> atDepth(0);
		oldTree->GetBodiesAtDepth(atDepth, depth);
		for(int j = 0; j < atDepth.size(); j++) {
			toReturn->Insert(atDepth[j]);
		}
	}

	// Perform release...
	delete oldTree;
	oldTree = 0;
	return toReturn;
}