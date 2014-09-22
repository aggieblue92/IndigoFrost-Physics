#ifndef FROST_COLLISION_MANAGER_INTERFACE_H
#define FROST_COLLISION_MANAGER_INTERFACE_H

#include "IPhysicsNode.h"
#include "IContact.h"
#include <vector>

/////////////////////////////////////////
// ICollisionManager: All physics nodes
//  are added via this interface, and
//  gross collision detection is done here.
/////////////////////////////////////////

namespace Frost
{
	class ICollisionManager
	{
	public:
		virtual void addPhysicsNode(IPhysicsNode*) = 0;
		virtual void removePhysicsNode(IPhysicsNode*) = 0;
		virtual void removePhysicsNode(std::string) = 0;
		virtual void genContacts(std::vector<IContact*>&) = 0;

		virtual void update(float) = 0;
	};
}

#endif