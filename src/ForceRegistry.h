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

#ifndef FROST_FORCE_REGISTRY_H
#define FROST_FORCE_REGISTRY_H

#include "IForce.h"
#include "IPhysicsNode.h"
#include <vector>

namespace Frost
{
	struct ForceEntry
	{
		IForce* _force;
		IPhysicsNode* _obj;
		ForceEntry(IForce*, IPhysicsNode*);
	};

	class ForceRegistry
	{
	public:
		ForceRegistry();
		
		// Add a force/body pair to the registry.
		//  Set either value to 0 to specify that it is applies to all objects of that type.
		void add(IForce* force, IPhysicsNode* physicsObject);

		// Remove a force/body pair from the registry.
		//  Set either value to 0 to specify that it applies to all objects of that type in the registry.
		bool remove(IForce* force, IPhysicsNode* physicsObject);

		unsigned int size() const;

		ForceEntry& operator[](unsigned int i);

		void clearRegistry();
		
		void updateForces(float timeElapsed);

	protected:
		std::vector<ForceEntry> _forcesList;
	};
}

#endif