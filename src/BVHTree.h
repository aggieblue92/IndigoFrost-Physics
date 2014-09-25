#ifndef FROST_COLLISION_MANAGER_BVH_TREE_H
#define FROST_COLLISION_MANAGER_BVH_TREE_H

/////////////////////////////////////////
// BVHTree: A collision manager that uses
//  a bounding volume heirarchy to perform
//  collision detection in a world.
/////////////////////////////////////////

#include "ICollisionManager.h"
#include "BVHNode.h"

namespace Frost
{
	class BVHTree : public ICollisionManager
	{
	public:
		BVHTree();
		~BVHTree();

		virtual void addPhysicsNode(IPhysicsNode*);
		virtual void removePhysicsNode(IPhysicsNode*);
		virtual void removePhysicsNode(std::string);
		virtual void genContacts(std::vector<IContact*>&);

		virtual void update(float);

	private:
		BVHNode* _root;

		void genContacts(std::vector<IContact*>& o_contactList, BVHNode* l, BVHNode* r);
		//std::thread treeUpdatingThread;
	};
}

#endif