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
		void Add(IForce* force, IPhysicsNode* physicsObject);

		// Remove a force/body pair from the registry.
		//  Set either value to 0 to specify that it applies to all objects of that type in the registry.
		bool Remove(IForce* force, IPhysicsNode* physicsObject);

		unsigned int size() const;

		ForceEntry& operator[](unsigned int i);

		void ClearRegistry();

		void UpdateForces(float timeElapsed);

	protected:
		std::vector<ForceEntry> _forcesList;
	};
}

#endif