/*
This source file is part of the Indigo Frost physics engine

The MIT License (MIT)

Copyright (c) 2014 Kamaron Peterson (aggieblue92)

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

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