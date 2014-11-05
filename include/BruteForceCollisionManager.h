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

#ifndef FROST_COLLISION_MANAGER_BRUTE_FORCE_H
#define FROST_COLLISION_MANAGER_BRUTE_FORCE_H

#include "DebugLogger.h"
#include "ICollisionManager.h"
#include <sstream>

/////////////////////////////////////////
// BruteForceCollisionManager: Collision
//  manager to use with debugging, does no
//  coarse collision detection.
/////////////////////////////////////////

namespace Frost
{
	class BruteForceCollisionManager : public ICollisionManager
	{
	public:
		BruteForceCollisionManager();
		
		virtual void addPhysicsNode(std::shared_ptr<IPhysicsNode>);
		virtual void removePhysicsNode(std::shared_ptr<IPhysicsNode>);
		virtual void removePhysicsNode(std::string);
		virtual void genContacts(std::vector<std::shared_ptr<IContact>>&);

		virtual void update(float dt);

	private:
		std::vector<std::shared_ptr<IPhysicsNode>> _objList;
	};
}

#endif
