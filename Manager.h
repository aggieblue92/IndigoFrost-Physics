#ifndef INDIGO_FROST_MANAGER_H
#define INDIGO_FROST_MANAGER_H

#include <vector>
#include "RigidBody.h"
#include "Forces.h"
#include "CollisionGeometry.h"
#include "Collisions.h"
#include "BVTNode.h"

// Manages all physics interactions in the world...
// IDEA: Make this smart, aim for a certain frame rate?

namespace Frost {
	class IndigoWorld {
	private:
		ForceRegistry* m_forces; // Force manager for world
		std::vector<RigidBody*> m_rbList; // List of rigid bodies in our world
		std::vector<BoundingSphere*> m_bvList; // List of bounding volumes attached to rigid bodies.
		std::vector<Contact*> m_contactList;
		BVTNode* m_bvhTree; // Bounding volume heirarchy for coarse collision detection

		int m_targetFrameSpeed; // Time in seconds that we are shooting for passing at a maximum
		int m_contactLimit; // Number of contacts allowable
		float m_contactResolveRate; // Rate at which extra contact resolution steps are allowed - 1 means only resolve generated contacts, no more, 2 means 2x ct... etc...

	public:
		IndigoWorld();
		~IndigoWorld();

		// Returns number of rigid bodies in the system
		int getNumRigidBodies();
		// Return target frame rate of engine
		int getTargetFrameRate();
		
		// Set target frame rate of system
		void setTargetFrameRate(int framesPerSecond);
		// Set target time elapsed per frame
		void setTargetFrameTime(int frameTime);
		// Set percent extra resolutions allowable - 0 means no extra steps.
		void setExtraResolutionStepAllowance(float pecentage);

		// Get information about a rigid body
		RigidBody* getRigidBodyInformation(int index);
		// Add a new rigid body to the system, return its index.
		int addNewRigidBody(const RigidBody& info);

		// Add a new force to the environment
		void addForce(RigidBody* rb, RigidBodyForceGenerator* force);
		// Add a new force to the force specified by index
		void addForce(int index, RigidBodyForceGenerator* force);
		// Remove force. If a force is found, return true. If either parameter is zero, serves as a wildcard.
		bool removeForce(RigidBody* rb, RigidBodyForceGenerator* force);

		// Simulate passing of world by timeElapsed seconds internally.
		void UpdateWorld(float timeElapsed);
	};
}

#endif
