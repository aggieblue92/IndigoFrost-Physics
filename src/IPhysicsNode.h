#ifndef FROST_PHYSICS_NODE_INTERFACE_H
#define FROST_PHYSICS_NODE_INTERFACE_H

/////////////////////////////////////////
// IPhysicsNode: Interface that holds
//  a node in the physics scene graph.
/////////////////////////////////////////

#include "IPhysicsObject.h"
#include "Collidable.h"
#include "FrostExceptions.h"
#include <string>

namespace Frost
{
	class IPhysicsNode
	{
	public:
		IPhysicsNode(IPhysicsObject* attachedObject = 0, Collidable* collisionInformation = 0, std::string name = "");
		~IPhysicsNode();

		operator IPhysicsObject*();

		IPhysicsObject* getPhysicsObject();
		Collidable* getCollidableData();

		std::string getName() const;
		void setName(std::string newName);

	protected:
		IPhysicsObject* _obj;
		Collidable* _collidableData;
		std::string _name;
	};
}

#endif