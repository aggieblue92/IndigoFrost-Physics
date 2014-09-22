#ifndef FROST_PHYSICS_NODE_INTERFACE_H
#define FROST_PHYSICS_NODE_INTERFACE_H

#include "IPhysicsObject.h"
#include "FrostExceptions.h"
#include <string>

namespace Frost
{
	class IPhysicsNode
	{
	public:
		IPhysicsNode(IPhysicsObject* attachedObject, std::string name = "");

		operator IPhysicsObject*();

		IPhysicsObject* getPhysicsObject();

		std::string getName() const;
		void setName(std::string newName);

	private:
		IPhysicsObject* _obj;
		std::string _name;
	};
}

#endif