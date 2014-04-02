#include "Manager.h"
using namespace Frost;

IndigoWorld::IndigoWorld()
: m_forces(new ForceRegistry()),
m_rbList(0),
m_bvhTree(0),
m_targetFrameSpeed(0.04),
m_contactLimit(50),
m_contactResolveRate(1.4f),
m_contactList(0)
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
		m_bvhTree->Release();
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

int IndigoWorld::addNewRigidBody(const RigidBody& info) {
	// Add to rigid body list...
	m_rbList.push_back(new RigidBody(info));
	int newPos = m_rbList.size() - 1;

	// Generate bounding volume sphere for rigid body...
	// UPDATE THIS HERE PLZ. Optimize, efficientcy-ize!
	// Origin: Origin of rigid body.
	// Radius: Distance of furthest point from origin on geometry
	float maxRadius = 1.f;
	for (int i = 0; i < info.m_collisionGeometry.size(); i++) {
		if (info.m_collisionGeometry[i]->getType() == GEOMETRY_TYPE::BOX) {
			// For now, while we're only using one collision object,
			//  the radius will just be distance of half size.
			Box* myBox;
			info.m_collisionGeometry[i]->fillBox(myBox);
			if (myBox != 0) {
				maxRadius = myBox->getHalfSize().Magnitude();
			}
		}
		else {
			Sphere* mySphere;
			info.m_collisionGeometry[i]->fillSphere(mySphere);
			if (mySphere != 0) {
				maxRadius = mySphere->getRadius();
			}
		}
	}
	m_bvList.push_back(new BoundingSphere(info.getPosition(), maxRadius));
	
	if (m_bvhTree == 0) {
		m_bvhTree = new BVTNode(0, m_bvList[newPos], m_rbList[newPos]);
	}
	else {
		m_bvhTree->Insert(m_rbList[newPos], m_bvList[newPos]);
	}

	return newPos;
}

// TODO: Add CUDA support?
void IndigoWorld::UpdateWorld(float timeElapsed) {
	int nContactsGenerated = 0;

	// First: Update all forces and integrators.
	m_forces->UpdateForces(timeElapsed);

	// Integrate rigid body data...
	// Update bounding sphere locations...
	for (int i = 0; i < m_rbList.size(); i++) {
		m_rbList[i]->Integrate(timeElapsed);
		m_bvList[i]->setLocation(m_rbList[i]->getPosition());
	}

	// Generate coarse contact data from tree...
	// TODO: Use a vector instead of an array?
	CoarseContact* myList = new CoarseContact[m_contactLimit];
	nContactsGenerated = m_bvhTree->CoarseCollisionDetect(myList, m_contactLimit);

	// Fine contact generation loop
	// Go through, adding contacts if they exist. Only add up until the limit.
	std::vector<Contact*> contactList(0);
	for (int i = 0; (i < m_contactLimit) && (contactList.size() <= (m_contactLimit * m_contactResolveRate)); i++) {
		// Check collision against all collision geometry in list.
		//  OPTIMIZE THIS BIT FOR MULTIPLE GEOMETRY
		if (myList[i].rb1 == 0 || myList[i].rb2 == 0)
			break;

		if (myList[i].rb1->getCollisionObject(0) == 0 || myList[i].rb2->getCollisionObject(0) == 0)
			break;

		myList[i].rb1->getCollisionObject(0)->genContacts(myList[i].rb2->getCollisionObject(0), contactList);
	}

	// Resolve all those contacts and clear them
	for (int i = 0; i < contactList.size(); i++) {
		// Version 1 algorithm: Just treat it like it's compressed
		//  the object and the object is bouncing back as if a spring.
		// For complicated simulations, this may not work so well, but
		//  oh well.
		
		// DEBUG RIGHT HERE - this isn't working.
		int k = 3500000; // in N/m
		contactList[i]->rb[0]->addForceAtPoint(contactList[i]->contactNormal_wd * contactList[i]->magnitude * (-k / timeElapsed), contactList[i]->contactPoint_wp);
		contactList[i]->rb[1]->addForceAtPoint(contactList[i]->contactNormal_wd * contactList[i]->magnitude * (k / timeElapsed), contactList[i]->contactPoint_wp);

		delete contactList[i];
		contactList[i] = 0;
	}
	contactList.resize(0);
}

void IndigoWorld::addForce(RigidBody* rb, RigidBodyForceGenerator* force) {
	m_forces->Add(rb, force);
}

void IndigoWorld::addForce(int index, RigidBodyForceGenerator* force) {
	if(index < m_rbList.size() && index >= 0)
		m_forces->Add(m_rbList[index], force);
}

bool IndigoWorld::removeForce(RigidBody* rb, RigidBodyForceGenerator* force) {
	return m_forces->Remove(rb, force);
}