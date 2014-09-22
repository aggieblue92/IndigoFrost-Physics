#ifndef FROST_COLLISION_MANAGER_BVH_TREE_H
#define FROST_COLLISION_MANAGER_BVH_TREE_H

#include "ICollisionManager.h"

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
	};
}

#endif