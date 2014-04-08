#include "Manager.h"
#include <iostream>
using namespace Frost;

IndigoWorld::IndigoWorld()
: m_forces(new ForceRegistry()),
m_rbList(0),
m_bvhTree(new BVTNode()),
m_targetFrameSpeed(0.04),
m_contactLimit(50),
m_contactResolveRate(1.4f),
m_contactList(0),
m_stop(false) // <BROKEN>,
//m_timeElapsedSinceRebalance(0.f),
//m_reBalanceRate(5.f)
//// </BROKEN>
{}

// TODO: Replace with shutdown() function instead?
IndigoWorld::~IndigoWorld() {
	// Shutdown force manager...
	if (m_forces != 0) {
		m_forces->Shutdown();
		delete m_forces;
		m_forces = 0;
	}

	// Shutdown bounding volume heirarchy tree...
	if (m_bvhTree != 0) {
		delete m_bvhTree;
		m_bvhTree = 0;
	}

	// Release all rigid body information...
	for (int i = 0; i < m_rbList.size(); i++) {
		if (m_rbList[i] != 0) {
			delete m_rbList[i];
			m_rbList[i] = 0;
		}
	}
	m_rbList.resize(0);
}

int IndigoWorld::getNumRigidBodies() {
	return m_rbList.size();
}

int IndigoWorld::getTargetFrameRate() {
	return 1 / m_targetFrameSpeed;
}

void IndigoWorld::setTargetFrameRate(int fps) {
	if (fps > 0)
		m_targetFrameSpeed = 1 / fps;
	else
		m_targetFrameSpeed = 50.f;
}

void IndigoWorld::setTargetFrameTime(int fL) {
	if (fL > 0)
		m_targetFrameSpeed = fL;
}

void IndigoWorld::setExtraResolutionStepAllowance(float percentage) {
	if (percentage >= 0.f)
		m_contactResolveRate = 1.f + percentage;
}

RigidBody* IndigoWorld::getRigidBodyInformation(int index) {
	if (m_rbList.size() > index)
		return m_rbList[index];
	else
		return 0;
}

int IndigoWorld::addNewRigidBody(RigidBody* info) {
	// Add to rigid body list...
	m_rbList.push_back(info);
	int newPos = m_rbList.size() - 1;

	// Insert to our tree...
	m_bvhTree->Insert(m_rbList.back());

	return newPos;
}

// TODO: Add CUDA support?
void IndigoWorld::UpdateWorld(float timeElapsed) {

	int nContactsGenerated = 0;

	//// <BROKEN>
	//m_timeElapsedSinceRebalance += timeElapsed;
	//if(m_timeElapsedSinceRebalance > m_reBalanceRate) {
	//	// TODO: This NEEDS to be handled using threading!
	//	m_bvhTree = BVTNode::createRebalancedTree(m_bvhTree);
	//	m_timeElapsedSinceRebalance = 0.f;
	//}
	//// </BROKEN>

	// First: Update all forces and integrators.
	m_forces->UpdateForces(timeElapsed);

	// Integrate rigid body data...
	// Update bounding sphere locations...
	for (int i = 0; i < m_rbList.size(); i++) {
		m_rbList[i]->Integrate(timeElapsed);
	}

	// <DEBUG>
	if ((m_rbList[0]->getPosition() - m_rbList[1]->getPosition()).Magnitude() < 1.5f) {
		bool breakHere = true;
	}
	// </DEBUG>

	m_bvhTree->Update();

	// Generate coarse contact data from tree...
	std::vector<CoarseContact> myList(0);
	m_bvhTree->CoarseCollision(myList, m_contactLimit);

	// Fine contact generation loop
	// Go through, adding contacts if they exist. Only add up until the limit.
	std::vector<Contact*> contactList(0);

	for (int i = 0; (i < m_contactLimit) && (i < myList.size()); i++) {
		// TODO: Create new contacts should they arise after resolution?

		// Check collision against all collision geometry in list.
		//  OPTIMIZE THIS BIT FOR MULTIPLE GEOMETRY
		if (myList[i].rb1->getCollisionObject(0) == 0 || myList[i].rb2->getCollisionObject(0) == 0)
			break;

		// Update fine data for collision objects...
		myList[i].rb1->getCollisionObject(0)->updateFineMatrix();
		myList[i].rb2->getCollisionObject(0)->updateFineMatrix();
		//for(int i = 0; myList[i].rb1->getCollisionObject(i) != 0; i++) {
		//	myList[i].rb1->getCollisionObject(i)->updateFineMatrix();
		//}
		//for(int i = 0; myList[i].rb2->getCollisionObject(i) != 0; i++) {
		//	myList[i].rb2->getCollisionObject(i)->updateFineMatrix();
		//}
		if(myList[i].rb1->getCollisionObject(0)->genContacts(myList[i].rb2->getCollisionObject(0), contactList)) {
			
		}
	}

	// Resolve all those contacts and clear them
	for (int i = 0; i < contactList.size(); i++) {
		// Version 1 algorithm: Just treat it like it's compressed
		//  the object and the object is bouncing back as if a spring.
		// For complicated simulations, this may not work so well, but
		//  oh well.
		
		// This bit is a hair wonky, you might want to fix it. You will want to fix it.
		int k = 70; // in N/m
		contactList[i]->rb[0]->addForceAtPoint(contactList[i]->contactNormal_wd * contactList[i]->magnitude * (k), contactList[i]->contactPoint_wp);
		contactList[i]->rb[1]->addForceAtPoint(contactList[i]->contactNormal_wd * contactList[i]->magnitude * (-k), contactList[i]->contactPoint_wp);

		delete contactList[i];
		contactList[i] = 0;
	}
	contactList.resize(0);
}

void IndigoWorld::addForce(RigidBody* rb, RigidBodyForceGenerator* force) {
	m_forces->Add(rb, force);
}

void IndigoWorld::setContactResolutionStepsAllowed(int nPerFrame) {
	if(nPerFrame > 0) {
		m_contactLimit = nPerFrame;
	}
}

void IndigoWorld::addForce(int index, RigidBodyForceGenerator* force) {
	if(index < m_rbList.size() && index >= 0)
		m_forces->Add(m_rbList[index], force);
}

bool IndigoWorld::removeForce(RigidBody* rb, RigidBodyForceGenerator* force) {
	return m_forces->Remove(rb, force);
}

bool IndigoWorld::removeForce(int index, RigidBodyForceGenerator* force) {
	if(index < m_rbList.size() && index >= 0)
		return m_forces->Remove(m_rbList[index], force);
	else
		return false;
}

// BEGIN DEBUG FUNCTIONS
void IndigoWorld::GetCirclesAtBVHLevel(int level, std::vector<Vect3>& o_origins, std::vector<float>& o_radii) {
	// Go into the BHV Tree, get bounding volume information at specified levels.
	std::vector<BoundingSphere*> list(0);
	m_bvhTree->GetNodesAtDepth(list, level);

	// Push out.
	for(int i = 0; i < list.size(); i++) {
		o_origins.push_back(list[i]->getLocation());
		o_radii.push_back(list[i]->getRadius());
	}
}