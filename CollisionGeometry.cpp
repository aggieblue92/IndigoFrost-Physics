#include "CollisionGeometry.h"
#include <vector>
using namespace Frost;

Geometry::Geometry(RigidBody* parent, enum GEOMETRY_TYPE type) {
	m_transform.setOrientationAndPos(Quaternion(), Vect3());
	m_type = type;
	m_parent = parent;
}

Geometry::Geometry(RigidBody* parent, const Matrix4& transform, enum GEOMETRY_TYPE type) {
	m_transform = transform;
	m_type = type;
	m_parent = parent;
}

Geometry::Geometry(RigidBody* parent, const Vect3& pos, const Quaternion& quat, enum GEOMETRY_TYPE type) {
	m_transform.setOrientationAndPos(quat, pos);
	m_type = type;
	m_parent = parent;
}

Matrix4 Geometry::getTransformMatrix() const {
	return m_transform;
}

void Geometry::setTransformMatrix(const Matrix4& o) {
	m_transform = o;
}

Vect3 Geometry::getPosition() const {
	return Vect3(m_transform.data[3], m_transform.data[7], m_transform.data[11]);
}

enum GEOMETRY_TYPE Geometry::getType() const {
	return m_type;
}

bool Geometry::genContacts(Geometry* g, std::vector<Contact*> o_list) const {
	if (g->m_type == GEOMETRY_TYPE::BOX) {
		Box* box;
		this->fillBox(box);
		return this->genContacts(box, o_list);
	}
	else if (g->m_type == GEOMETRY_TYPE::SPHERE) {
		Sphere* sp;
		this->fillSphere(sp);
		return this->genContacts(sp, o_list);
	}
}

// Begin SPHERE code:
Sphere::Sphere(RigidBody* parent)
	: Geometry(parent, Vect3(), Quaternion(), GEOMETRY_TYPE::SPHERE), m_radius(0.f)
{}

Sphere::Sphere(RigidBody* parent, Vect3 pos, float radius)
	: Geometry(parent, pos, Quaternion(), GEOMETRY_TYPE::SPHERE), m_radius(radius)
{}

Sphere::Sphere(const Sphere& o)
	: Geometry(m_parent, o.m_transform, GEOMETRY_TYPE::SPHERE), m_radius(o.m_radius)
{}

float Sphere::getRadius() {
	return m_radius;
}

void Sphere::setRadius(float radius) {
	if(radius >= 0.f)
		m_radius = radius;
}

// Fill sphere object with data from this class.
void Sphere::fillSphere(Sphere*& s) const {
	s = new Sphere(*this);
}

// Fill the box object with NULL.
void Sphere::fillBox(Box*& b) const {
	b = 0;
}

// Is the sphere touching the other sphere?
bool Sphere::isTouching(Sphere* s) const {
	// The distance is the difference between
	//  the two objects.
	Vect3 distance = s->getPosition() - this->getPosition();

	// The spheres are touching if the magnitude of the distance
	//  is less than the sum of the radii.
	return distance.Magnitude() <= (s->m_radius + m_radius);
}

bool Sphere::genContacts(Sphere* s, std::vector<Contact*> o_list) const {
	// Get distance between two objects, with this one at origin...
	Vect3 distance = s->getPosition() - this->getPosition();

	// Spheres are touching if the magnitude of the distance
	//  is less than the sum of the radii.
	if (distance.Magnitude() > (s->m_radius + m_radius))
		return false;
	else {
		// Generate contact! Contact is midpoint in world space
		//  of intersection, normal is direction of intersection.
		Vect3 dirNorm = (s->getPosition() - this->getPosition()).GetNormal();
		
		Vect3 contactNormal = ((s->getPosition() - (dirNorm * s->getRadius())) - (this->getPosition() + (dirNorm * this->m_radius)));
		Vect3 contactPoint = contactNormal * 0.5;
		float contactMagnitude = contactNormal.Magnitude();
		contactNormal *= (1.f / contactMagnitude);

		// TODO: Update this to include friction and restitution properly!!!
		o_list.push_back(new Contact(m_parent, s->m_parent, contactPoint, contactNormal, contactMagnitude, 0.8f));

		return true;
	}
}

// Is this sphere touching the box?
bool Sphere::isTouching(Box* b) const {
	// First, we need to transform the sphere position
	//  into local coordinates of the box.
	Vect3 transformedSpherePosition = b->getTransformMatrix() * this->getPosition();

	// Now, we need to find the closest point on the cube.
	//  For any axis, it will be the cube half-length on that
	//  side if the sphere's position is further than the half-length,
	//  or it will be the component of the sphere's location on that
	//  axis if not.
	Vect3 closestBoxPoint = transformedSpherePosition;
	if(closestBoxPoint.x > b->getHalfSize().x)
		closestBoxPoint.x = b->getHalfSize().x;
	else if(closestBoxPoint.x < -b->getHalfSize().x)
		closestBoxPoint.x = -b->getHalfSize().x;

	if(closestBoxPoint.y > b->getHalfSize().y)
		closestBoxPoint.y = b->getHalfSize().y;
	else if(closestBoxPoint.y < -b->getHalfSize().y)
		closestBoxPoint.y = -b->getHalfSize().y;

	if(closestBoxPoint.z > b->getHalfSize().z)
		closestBoxPoint.z = b->getHalfSize().z;
	else if(closestBoxPoint.z < -b->getHalfSize().z)
		closestBoxPoint.z = -b->getHalfSize().z;

	// Now we have the closest point on the box.
	if ((closestBoxPoint - transformedSpherePosition).Magnitude() <= m_radius) {
		// OUT-CONTACT WRITE
		Vect3 penetrationNormal(closestBoxPoint - transformedSpherePosition);
		penetrationNormal -= (penetrationNormal.GetNormal() * m_radius);
		float penetrationMagnitude = penetrationNormal.Magnitude();
		penetrationNormal *= 1.f / penetrationMagnitude;
		// OUT-CONTACT DATA READY
	}
	return (closestBoxPoint - transformedSpherePosition).Magnitude() <= m_radius;
}

bool Sphere::genContacts(Box* b, std::vector<Contact*> o_list) const {
	// First, we need to transform the sphere position
	//  into local coordinates of the box.
	Vect3 transformedSpherePosition = b->getTransformMatrix() * this->getPosition();

	// Now, we need to find the closest point on the cube.
	//  For any axis, it will be the cube half-length on that
	//  side if the sphere's position is further than the half-length,
	//  or it will be the component of the sphere's location on that
	//  axis if not.
	Vect3 closestBoxPoint = transformedSpherePosition;
	if (closestBoxPoint.x > b->getHalfSize().x)
		closestBoxPoint.x = b->getHalfSize().x;
	else if (closestBoxPoint.x < -b->getHalfSize().x)
		closestBoxPoint.x = -b->getHalfSize().x;

	if (closestBoxPoint.y > b->getHalfSize().y)
		closestBoxPoint.y = b->getHalfSize().y;
	else if (closestBoxPoint.y < -b->getHalfSize().y)
		closestBoxPoint.y = -b->getHalfSize().y;

	if (closestBoxPoint.z > b->getHalfSize().z)
		closestBoxPoint.z = b->getHalfSize().z;
	else if (closestBoxPoint.z < -b->getHalfSize().z)
		closestBoxPoint.z = -b->getHalfSize().z;

	// Now we have the closest point on the box.
	if ((closestBoxPoint - transformedSpherePosition).Magnitude() <= m_radius) {
		// OUT-CONTACT WRITE
		Vect3 penetrationPoint(transformedSpherePosition - closestBoxPoint);
		penetrationPoint -= (penetrationPoint.GetNormal() * m_radius);
		o_list.push_back(new Contact(m_parent, b->getParent(),
			b->getTransformMatrix().inverse() * closestBoxPoint,
			b->getTransformMatrix().inverse().transformDirection(penetrationPoint.GetNormal()),
			penetrationPoint.Magnitude(), 0.8f));
		float penetrationMagnitude = penetrationPoint.Magnitude();
		penetrationPoint *= 1.f / penetrationMagnitude;
		// OUT-CONTACT DATA READY

		return true;
	}
	return (closestBoxPoint - transformedSpherePosition).Magnitude() <= m_radius;
}

Box::Box(RigidBody* parent)
	: Geometry(parent, GEOMETRY_TYPE::BOX), m_halfSize(Vect3())
{}

Box::Box(RigidBody* parent, Vect3 pos, Quaternion rot, Vect3 halfSize)
	: Geometry(parent, pos, rot, GEOMETRY_TYPE::BOX), m_halfSize(halfSize)
{}

Box::Box(RigidBody* parent, Matrix4 transform, Vect3 halfSize)
	: Geometry(parent, transform, GEOMETRY_TYPE::BOX), m_halfSize(halfSize)
{}

Box::Box(const Box& other)
	: Geometry(m_parent, other.m_transform, GEOMETRY_TYPE::BOX), m_halfSize(other.m_halfSize)
{}

Vect3 Box::getHalfSize() const {
	return m_halfSize;
}

// POSSIBLE UPGRADE: Negative numbers OK, they are just turned positive.
void Box::setHalfSize(float x, float y, float z) {
	if(x >= 0.f && y >= 0.f && z >= 0.f) {
		m_halfSize.x = x;
		m_halfSize.y = y;
		m_halfSize.z = z;
	}
}

void Box::setHalfSize(Vect3 halfSize) {
	if(halfSize.x < 0.f || halfSize.y < 0.f || halfSize.z < 0.f)
		return;
	m_halfSize = halfSize;
}

void Box::fillSphere(Sphere*& s) const {
	s = 0;
}

void Box::fillBox(Box*& b) const {
	b = new Box(*this);
}

RigidBody* Box::getParent() {
	return m_parent;
}

bool Box::isTouching(Sphere* s) const {
	Box toPass(*this);
	return s->isTouching(&toPass);
}

bool Box::genContacts(Sphere* s, std::vector<Contact*> o_list) const {
	Box toPass(*this);
	return s->genContacts(&toPass, o_list);
}

bool Box::isTouching(Box* b) const {
	// Two types of detection we're going to test for: edge-edge and corner-face.
	//  All other types will resolve to another, or can be comprised of the others.
	
	// Get world position of two boxes...
	Vect3 myPos_ws = this->getPosition();
	Vect3 otherPos_ws = b->getPosition();
	Vect3 direction_ws = (b->getPosition() - this->getPosition()).GetNormal();

	// Optimization: This whole thing can be threaded OR use CUDA cores.
	//  Your pick, really.

	//////////////////// POINT-FACE DETECTION /////////////////////////
	// Find vertices which are in a positive direction towards the other box,
	//  and also the ones on the other box that are in our direction.
	std::vector<Vect3> myQualifying_ws(0);
	std::vector<Vect3> otherQualifying_ws(0);

	// Optimization - right now it will almost always return around 4 vectors from each of these functions - that
	//  means 8 point checks and lots and lots of edge checks. Try to reduce this number.
	for (int i = 0; i < 8; i++) {
		// Check on ourselves...
		Vect3 check_ls = Vect3(
			(i % 2 == 0 ? -1 : 1) * this->m_halfSize.x,
			((i / 2) % 2 == 0 ? -1 : 1) * this->m_halfSize.y,
			((i / 4) % 2 == 0 ? -1 : 1) * this->m_halfSize.z);
		Vect3 direction_ls = this->m_transform.inverse().transformDirection(direction_ws); // Make sure?
		if (check_ls * direction_ls > 0.f) {
			myQualifying_ws.push_back(this->m_transform * check_ls);
		}
		
		// Check on other box...
		check_ls = Vect3(
			(i % 2 == 0 ? -1 : 1) * b->m_halfSize.x,
			((i / 2) % 2 == 0 ? -1 : 1) * b->m_halfSize.y,
			((i / 4) % 2 == 0 ? -1 : 1) * b->m_halfSize.z);
		direction_ls = b->m_transform.inverse().transformDirection(direction_ws);
		// Go in the other direction now, since the direction_ws vector is from this to other.
		if (check_ls * direction_ls < 0.f) {
			otherQualifying_ws.push_back(b->m_transform * check_ls);
		}
	}

	// First, check point-face collisions. This is done by examining the point in local space,
	//  to see if it is within all three half-size parameters.
	for (int i = 0; i < myQualifying_ws.size(); i++) {
		Vect3 checkPoint = b->m_transform.inverse() * myQualifying_ws[i];
		if ((abs(checkPoint.x) <= b->m_halfSize.x) &&
			(abs(checkPoint.y) <= b->m_halfSize.y) &&
			(abs(checkPoint.z) <= b->m_halfSize.z)) {
			// Collision at this point - collision normal is direction to closest
			//  face, penetration is distance to that point.
			return true;
		}
	}

	for (int i = 0; i < otherQualifying_ws.size(); i++) {
		Vect3 checkPoint = this->m_transform.inverse() * otherQualifying_ws[i];
		if ((abs(checkPoint.x) <= this->m_halfSize.x) &&
			(abs(checkPoint.y) <= this->m_halfSize.y) &&
			(abs(checkPoint.z) <= this->m_halfSize.z)) {
			// Collision at this point - collision normal is direction to closest
			//  face, penetration is distance to that point.
			return true;
		}
	}

	///////////////////// EDGE EDGE DETECTION ////////////////////////
	// Now to pick the edges. 
	// Look at all 12 edges. If both vertices are not in the direction of the other
	//  box, don't consider that edge.
	// Otherwise, add it to the list.
	
	// 12 edges - pick two directions, the edge will be formed by the two vertices on either side.
	//  e.g., pick ++, edge is +++/++-.
	// Pick all edges in world space.
	std::vector<Vect3> our_edge_list_ws(0);
	std::vector<Vect3> other_edge_list_ws(0);
	for (int i = 0; i < 12; i++) {
		// 0-3: variable first direction
		Vect3 pt_1_ls, opt_1_ls;
		Vect3 pt_2_ls, opt_2_ls;
		if (i / 4 == 0) {
			pt_1_ls = Vect3(this->m_halfSize.x,
				((i / 2) % 2 == 0 ? -1 : 1) * this->m_halfSize.y,
				(i % 2 == 0 ? -1 : 1) * this->m_halfSize.z);
			pt_2_ls = Vect3(-this->m_halfSize.x,
				((i / 2) % 2 == 0 ? -1 : 1) * this->m_halfSize.y,
				(i % 2 == 0 ? -1 : 1) * this->m_halfSize.z);
			opt_1_ls = Vect3(b->m_halfSize.x,
				((i / 2) % 2 == 0 ? -1 : 1) * b->m_halfSize.y,
				(i % 2 == 0 ? -1 : 1) * b->m_halfSize.z);
			opt_2_ls = Vect3(-b->m_halfSize.x,
				((i / 2) % 2 == 0 ? -1 : 1) * b->m_halfSize.y,
				(i % 2 == 0 ? -1 : 1) * b->m_halfSize.z);
		}
		// 4-7: variable second direction
		else if (i / 4 == 1) {
			pt_1_ls = Vect3(((i / 2) % 2 == 0 ? -1 : 1) * this->m_halfSize.x,
				this->m_halfSize.y,
				(i % 2 == 0 ? -1 : 1) * this->m_halfSize.z);
			pt_2_ls = Vect3(((i / 2) % 2 == 0 ? -1 : 1) * this->m_halfSize.x,
				-this->m_halfSize.y,
				(i % 2 == 0 ? -1 : 1) * this->m_halfSize.z);
			opt_1_ls = Vect3(((i / 2) % 2 == 0 ? -1 : 1) * b->m_halfSize.x,
				b->m_halfSize.y,
				(i % 2 == 0 ? -1 : 1) * b->m_halfSize.z);
			opt_2_ls = Vect3(((i / 2) % 2 == 0 ? -1 : 1) * b->m_halfSize.x,
				-b->m_halfSize.y,
				(i % 2 == 0 ? -1 : 1) * b->m_halfSize.z);
		}
		// 9-11: variable third direction
		else {
			pt_1_ls = Vect3(((i / 2) % 2 == 0 ? -1 : 1) * this->m_halfSize.x,
				(i % 2 == 0 ? -1 : 1) * this->m_halfSize.y,
				this->m_halfSize.z);
			pt_2_ls = Vect3(((i / 2) % 2 == 0 ? -1 : 1) * this->m_halfSize.x,
				(i % 2 == 0 ? -1 : 1) * this->m_halfSize.y,
				-this->m_halfSize.z);
			opt_1_ls = Vect3(((i / 2) % 2 == 0 ? -1 : 1) * b->m_halfSize.x,
				(i % 2 == 0 ? -1 : 1) * b->m_halfSize.y,
				b->m_halfSize.z);
			opt_2_ls = Vect3(((i / 2) % 2 == 0 ? -1 : 1) * b->m_halfSize.x,
				(i % 2 == 0 ? -1 : 1) * b->m_halfSize.y,
				-b->m_halfSize.z);
		}

		// Edges are selected. If both points are in direction of other cube,
		//  add them to the edge list.
		Vect3 dirn_ls = this->m_transform.inverse().transformDirection(direction_ws);
		if (pt_1_ls * dirn_ls > 0 && pt_2_ls * dirn_ls > 0) {
			our_edge_list_ws.push_back(this->m_transform * pt_1_ls);
			our_edge_list_ws.push_back(this->m_transform * pt_2_ls);
		}
		dirn_ls = b->m_transform.inverse().transformDirection(direction_ws * -1);
		if (opt_1_ls * dirn_ls > 0 && opt_2_ls * dirn_ls > 0) {
			other_edge_list_ws.push_back(b->m_transform * opt_1_ls);
			other_edge_list_ws.push_back(b->m_transform * opt_2_ls);
		}
		
		// SWEET JESUS this is getting long.
		//  Remember - CUDA, Threading or Single, this whole process needs to run in the range of microseconds, around 40.
		//  With limited resources because the graphics engine is doing things.
	}

	// Alright, our vectors are full.
	//  We need to check each edge against each other edge.
	for (int i = 0; i < our_edge_list_ws.size(); i += 2) {
		for (int j = 0; j < other_edge_list_ws.size(); j += 2) {
			// First off, find our edge-space vectors.
			Vect3 ourEdge = our_edge_list_ws[i + 1] - our_edge_list_ws[i];
			Vect3 otherEdge = other_edge_list_ws[j + 1] - other_edge_list_ws[j];

			float alpha1, alpha2; // These are our coefficients for finding the shortest distance vector between the edges.
			
			// Linear algebra time! If some point ourMiddleishPoint = ourEdgeOrigin_ws + (alpha)(ourEdge),
			//  and likewise for the otherMiddleishPoint, then (otherMiddleish - ourMiddleish) * ourEdge = 0
			//  and (otherMiddleish - ourMiddleish) * otherEdge = 0, then the vector
			//  distanceVector = (otherMiddleish - ourMiddleish) is the vector from the closest point on our edge
			//  to the closest point on the other edge. This is 0 at contact, and it is in the negative direction_ws
			//  direction if the edges are interpenetrating.
			
			// Doing math and solving for a 2x2 system of equations, you get this:
			/*
			[[ -r_1 * r_1    r_2 * r_1 ]] [[ a_1 ]]   =   [[ p_11 * r_1 - p_21 * r_1 ]]
			[[ r_1 * r_2    -r_2 * r_2 ]] [[ a_2 ]]       [[ p_21 * r_2 - p_11 * r_2 ]]
			r_1 := ourEdge                  a_1 := percentage of length of r_1 down which the closest point to the other edge is.
			r_2 := otherEdge                a_2 := percentage of length of r_2 down which the closest point to our edge is.
			p_11 := our_edge_list_ws[i]     p_21 := other_edge_list_ws[i]
			p_12 := our_edge_list_ws[i+1]   p_22 := other_edge_list_ws[i+1]
			*/
			// Solve the system (I found a closed form using Gauss' method).

			// WATCH OUT for when the lines are parallel - i.e., if the first matrix is non-invertible!
			//  In this case, just pick an a_1, and solve for the a_2.

			if (ourEdge * ourEdge + otherEdge * otherEdge == ourEdge * otherEdge * 2) {
				alpha1 = 0.5;
				// (p_11*r_1 - p_21*r_1 + 0.5r_1*r_1)/(r_2*r_1) = a_2
				alpha2 = ((our_edge_list_ws[i] * ourEdge - other_edge_list_ws[j] * ourEdge) + (ourEdge * ourEdge * alpha1)) / (otherEdge * ourEdge);
			}
			else {
				// No such luck. Now, to use the closed form...
				// First matrix coefficients - rows are [0] [1], cols [1] [2] (diagonal matrix)
				float a[3] = { -(ourEdge * ourEdge), ourEdge * otherEdge, -(otherEdge * otherEdge) };
				// Result vector - from the solution. Think of this as the b in Ax=b.
				float b[2] = { our_edge_list_ws[i] * ourEdge - other_edge_list_ws[j] * ourEdge,
					other_edge_list_ws[j] * otherEdge - our_edge_list_ws[i] * otherEdge };

				// Closed form for a_1 and a_2... BROKEN.
				alpha2 = (b[1] - b[0] * (a[1] / a[0])) / (a[2] - a[1] * (a[1] / a[0]));
				alpha1 = (b[0] - a[1] * alpha2) / a[0];/*
				alpha2 = (b[1] - (a[2] * b[0]) / a[0]) / (a[2] - (a[1] * a[1]) / a[0]);
				alpha1 = (b[0] - (a[1] / (a[2] - (a[1] * a[1]) / a[0])) * (b[1] - (a[1] * b[0]) / a[0])) / a[0];*/
			}

			// Now we have our alpha values. Early out if they are out of bounds.
			if (alpha1 < 0.f || alpha1 > 1.f || alpha2 > 1.f || alpha2 < 0.f)
				continue;

			// They are within bounds. Find vector from closest points...
			Vect3 edgeDistance_ws((other_edge_list_ws[j] + otherEdge * alpha2) - (our_edge_list_ws[i] + ourEdge * alpha1));

			// Check against world space direction vector between the two boxes. If the number is positive, get out.
			if (edgeDistance_ws * direction_ws > 0.f)
				continue;
			else {
				// Houston, we have collision.
				return true;
			}
		}
	}

	return false;
}

bool Box::genContacts(Box* b, std::vector<Contact*> o_list) const {
	// Two types of detection we're going to test for: edge-edge and corner-face.
	//  All other types will resolve to another, or can be comprised of the others.

	// Get world position of two boxes...
	Vect3 myPos_ws = this->getPosition();
	Vect3 otherPos_ws = b->getPosition();
	Vect3 direction_ws = (b->getPosition() - this->getPosition()).GetNormal();

	bool contactFound = false;

	// Optimization: This whole thing can be threaded OR use CUDA cores.
	//  Your pick, really.

	//////////////////// POINT-FACE DETECTION /////////////////////////
	// Find vertices which are in a positive direction towards the other box,
	//  and also the ones on the other box that are in our direction.
	std::vector<Vect3> myQualifying_ws(0);
	std::vector<Vect3> otherQualifying_ws(0);

	// Optimization - right now it will almost always return around 4 vectors from each of these functions - that
	//  means 8 point checks and lots and lots of edge checks. Try to reduce this number.
	for (int i = 0; i < 8; i++) {
		// Check on ourselves...
		Vect3 check_ls = Vect3(
			(i % 2 == 0 ? -1 : 1) * this->m_halfSize.x,
			((i / 2) % 2 == 0 ? -1 : 1) * this->m_halfSize.y,
			((i / 4) % 2 == 0 ? -1 : 1) * this->m_halfSize.z);
		Vect3 direction_ls = this->m_transform.inverse().transformDirection(direction_ws); // Make sure?
		if (check_ls * direction_ls > 0.f) {
			myQualifying_ws.push_back(this->m_transform * check_ls);
		}

		// Check on other box...
		check_ls = Vect3(
			(i % 2 == 0 ? -1 : 1) * b->m_halfSize.x,
			((i / 2) % 2 == 0 ? -1 : 1) * b->m_halfSize.y,
			((i / 4) % 2 == 0 ? -1 : 1) * b->m_halfSize.z);
		direction_ls = b->m_transform.inverse().transformDirection(direction_ws);
		// Go in the other direction now, since the direction_ws vector is from this to other.
		if (check_ls * direction_ls < 0.f) {
			otherQualifying_ws.push_back(b->m_transform * check_ls);
		}
	}

	// First, check point-face collisions. This is done by examining the point in local space,
	//  to see if it is within all three half-size parameters.
	for (int i = 0; i < myQualifying_ws.size(); i++) {
		Vect3 checkPoint_ls = b->m_transform.inverse() * myQualifying_ws[i];
		if ((abs(checkPoint_ls.x) <= b->m_halfSize.x) &&
			(abs(checkPoint_ls.y) <= b->m_halfSize.y) &&
			(abs(checkPoint_ls.z) <= b->m_halfSize.z)) {
			// Collision at this point - collision normal is direction to closest
			//  face, penetration is distance to that point.

			float xMagnitude = (checkPoint_ls.x > 0) ? b->m_halfSize.x - checkPoint_ls.x : checkPoint_ls.x + b->m_halfSize.x;
			float yMagnitude = (checkPoint_ls.y > 0) ? b->m_halfSize.y - checkPoint_ls.y : checkPoint_ls.y + b->m_halfSize.y;
			float zMagnitude = (checkPoint_ls.z > 0) ? b->m_halfSize.z - checkPoint_ls.z : checkPoint_ls.z + b->m_halfSize.z;

			// Direction of least penetration is the direction we'll choose
			if (xMagnitude < yMagnitude && xMagnitude < zMagnitude) {
				// Point is checkPoint in world space
				// Normal is checkPoint - halfSize
				o_list.push_back(new Contact(this->m_parent, b->m_parent,
					b->m_transform * checkPoint_ls,
					b->m_transform.transformDirection(Vect3(xMagnitude, 0.f, 0.f).GetNormal()),
					xMagnitude, 0.8f));
			}
			else if (yMagnitude < zMagnitude) {
				o_list.push_back(new Contact(this->m_parent, b->m_parent,
					b->m_transform * checkPoint_ls,
					b->m_transform.transformDirection(Vect3(0.f, yMagnitude, 0.f).GetNormal()),
					yMagnitude, 0.8f));
			}
			else {
				o_list.push_back(new Contact(this->m_parent, b->m_parent,
					b->m_transform * checkPoint_ls,
					b->m_transform.transformDirection(Vect3(0.f, 0.f, zMagnitude).GetNormal()),
					zMagnitude, 0.8f));
			}

			contactFound = true;
		}
	}

	for (int i = 0; i < otherQualifying_ws.size(); i++) {
		Vect3 checkPoint_ls = this->m_transform.inverse() * otherQualifying_ws[i];
		if ((abs(checkPoint_ls.x) <= this->m_halfSize.x) &&
			(abs(checkPoint_ls.y) <= this->m_halfSize.y) &&
			(abs(checkPoint_ls.z) <= this->m_halfSize.z)) {
			// Collision at this point - collision normal is direction to closest
			//  face, penetration is distance to that point.
			
			// Collision at this point - collision normal is direction to closest
			//  face, penetration is distance to that point.

			float xMagnitude = (checkPoint_ls.x > 0) ? this->m_halfSize.x - checkPoint_ls.x : checkPoint_ls.x + this->m_halfSize.x;
			float yMagnitude = (checkPoint_ls.y > 0) ? this->m_halfSize.y - checkPoint_ls.y : checkPoint_ls.y + this->m_halfSize.y;
			float zMagnitude = (checkPoint_ls.z > 0) ? this->m_halfSize.z - checkPoint_ls.z : checkPoint_ls.z + this->m_halfSize.z;

			// Direction of least penetration is the direction we'll choose
			if (xMagnitude < yMagnitude && xMagnitude < zMagnitude) {
				// Point is checkPoint in world space
				// Normal is checkPoint - halfSize
				o_list.push_back(new Contact(this->m_parent, this->m_parent,
					this->m_transform * checkPoint_ls,
					this->m_transform.transformDirection(Vect3(xMagnitude, 0.f, 0.f).GetNormal()),
					xMagnitude, 0.8f));
			}
			else if (yMagnitude < zMagnitude) {
				o_list.push_back(new Contact(this->m_parent, this->m_parent,
					this->m_transform * checkPoint_ls,
					this->m_transform.transformDirection(Vect3(0.f, yMagnitude, 0.f).GetNormal()),
					yMagnitude, 0.8f));
			}
			else {
				o_list.push_back(new Contact(this->m_parent, this->m_parent,
					this->m_transform * checkPoint_ls,
					this->m_transform.transformDirection(Vect3(0.f, 0.f, zMagnitude).GetNormal()),
					zMagnitude, 0.8f));
			}

			contactFound = true;
		}
	}

	///////////////////// EDGE EDGE DETECTION ////////////////////////
	// Now to pick the edges. 
	// Look at all 12 edges. If both vertices are not in the direction of the other
	//  box, don't consider that edge.
	// Otherwise, add it to the list.

	// 12 edges - pick two directions, the edge will be formed by the two vertices on either side.
	//  e.g., pick ++, edge is +++/++-.
	// Pick all edges in world space.
	std::vector<Vect3> our_edge_list_ws(0);
	std::vector<Vect3> other_edge_list_ws(0);
	for (int i = 0; i < 12; i++) {
		// 0-3: variable first direction
		Vect3 pt_1_ls, opt_1_ls;
		Vect3 pt_2_ls, opt_2_ls;
		if (i / 4 == 0) {
			pt_1_ls = Vect3(this->m_halfSize.x,
				((i / 2) % 2 == 0 ? -1 : 1) * this->m_halfSize.y,
				(i % 2 == 0 ? -1 : 1) * this->m_halfSize.z);
			pt_2_ls = Vect3(-this->m_halfSize.x,
				((i / 2) % 2 == 0 ? -1 : 1) * this->m_halfSize.y,
				(i % 2 == 0 ? -1 : 1) * this->m_halfSize.z);
			opt_1_ls = Vect3(b->m_halfSize.x,
				((i / 2) % 2 == 0 ? -1 : 1) * b->m_halfSize.y,
				(i % 2 == 0 ? -1 : 1) * b->m_halfSize.z);
			opt_2_ls = Vect3(-b->m_halfSize.x,
				((i / 2) % 2 == 0 ? -1 : 1) * b->m_halfSize.y,
				(i % 2 == 0 ? -1 : 1) * b->m_halfSize.z);
		}
		// 4-7: variable second direction
		else if (i / 4 == 1) {
			pt_1_ls = Vect3(((i / 2) % 2 == 0 ? -1 : 1) * this->m_halfSize.x,
				this->m_halfSize.y,
				(i % 2 == 0 ? -1 : 1) * this->m_halfSize.z);
			pt_2_ls = Vect3(((i / 2) % 2 == 0 ? -1 : 1) * this->m_halfSize.x,
				-this->m_halfSize.y,
				(i % 2 == 0 ? -1 : 1) * this->m_halfSize.z);
			opt_1_ls = Vect3(((i / 2) % 2 == 0 ? -1 : 1) * b->m_halfSize.x,
				b->m_halfSize.y,
				(i % 2 == 0 ? -1 : 1) * b->m_halfSize.z);
			opt_2_ls = Vect3(((i / 2) % 2 == 0 ? -1 : 1) * b->m_halfSize.x,
				-b->m_halfSize.y,
				(i % 2 == 0 ? -1 : 1) * b->m_halfSize.z);
		}
		// 9-11: variable third direction
		else {
			pt_1_ls = Vect3(((i / 2) % 2 == 0 ? -1 : 1) * this->m_halfSize.x,
				(i % 2 == 0 ? -1 : 1) * this->m_halfSize.y,
				this->m_halfSize.z);
			pt_2_ls = Vect3(((i / 2) % 2 == 0 ? -1 : 1) * this->m_halfSize.x,
				(i % 2 == 0 ? -1 : 1) * this->m_halfSize.y,
				-this->m_halfSize.z);
			opt_1_ls = Vect3(((i / 2) % 2 == 0 ? -1 : 1) * b->m_halfSize.x,
				(i % 2 == 0 ? -1 : 1) * b->m_halfSize.y,
				b->m_halfSize.z);
			opt_2_ls = Vect3(((i / 2) % 2 == 0 ? -1 : 1) * b->m_halfSize.x,
				(i % 2 == 0 ? -1 : 1) * b->m_halfSize.y,
				-b->m_halfSize.z);
		}

		// Edges are selected. If both points are in direction of other cube,
		//  add them to the edge list.
		Vect3 dirn_ls = this->m_transform.inverse().transformDirection(direction_ws);
		if (pt_1_ls * dirn_ls > 0 && pt_2_ls * dirn_ls > 0) {
			our_edge_list_ws.push_back(this->m_transform * pt_1_ls);
			our_edge_list_ws.push_back(this->m_transform * pt_2_ls);
		}
		dirn_ls = b->m_transform.inverse().transformDirection(direction_ws * -1);
		if (opt_1_ls * dirn_ls > 0 && opt_2_ls * dirn_ls > 0) {
			other_edge_list_ws.push_back(b->m_transform * opt_1_ls);
			other_edge_list_ws.push_back(b->m_transform * opt_2_ls);
		}

		// SWEET JESUS this is getting long.
		//  Remember - CUDA, Threading or Single, this whole process needs to run in the range of microseconds, around 40.
		//  With limited resources because the graphics engine is doing things.
	}

	// Alright, our vectors are full.
	//  We need to check each edge against each other edge.
	for (int i = 0; i < our_edge_list_ws.size(); i += 2) {
		for (int j = 0; j < other_edge_list_ws.size(); j += 2) {
			// First off, find our edge-space vectors.
			Vect3 ourEdge = our_edge_list_ws[i + 1] - our_edge_list_ws[i];
			Vect3 otherEdge = other_edge_list_ws[j + 1] - other_edge_list_ws[j];

			float alpha1, alpha2; // These are our coefficients for finding the shortest distance vector between the edges.

			// Linear algebra time! If some point ourMiddleishPoint = ourEdgeOrigin_ws + (alpha)(ourEdge),
			//  and likewise for the otherMiddleishPoint, then (otherMiddleish - ourMiddleish) * ourEdge = 0
			//  and (otherMiddleish - ourMiddleish) * otherEdge = 0, then the vector
			//  distanceVector = (otherMiddleish - ourMiddleish) is the vector from the closest point on our edge
			//  to the closest point on the other edge. This is 0 at contact, and it is in the negative direction_ws
			//  direction if the edges are interpenetrating.

			// Doing math and solving for a 2x2 system of equations, you get this:
			/*
			[[ -r_1 * r_1    r_2 * r_1 ]] [[ a_1 ]]   =   [[ p_11 * r_1 - p_21 * r_1 ]]
			[[ r_1 * r_2    -r_2 * r_2 ]] [[ a_2 ]]       [[ p_21 * r_2 - p_11 * r_2 ]]
			r_1 := ourEdge                  a_1 := percentage of length of r_1 down which the closest point to the other edge is.
			r_2 := otherEdge                a_2 := percentage of length of r_2 down which the closest point to our edge is.
			p_11 := our_edge_list_ws[i]     p_21 := other_edge_list_ws[i]
			p_12 := our_edge_list_ws[i+1]   p_22 := other_edge_list_ws[i+1]
			*/
			// Solve the system (I found a closed form using Gauss' method).

			// WATCH OUT for when the lines are parallel - i.e., if the first matrix is non-invertible!
			//  In this case, just pick an a_1, and solve for the a_2.

			if (ourEdge * ourEdge + otherEdge * otherEdge == ourEdge * otherEdge * 2) {
				alpha1 = 0.5;
				// (p_11*r_1 - p_21*r_1 + 0.5r_1*r_1)/(r_2*r_1) = a_2
				alpha2 = ((our_edge_list_ws[i] * ourEdge - other_edge_list_ws[j] * ourEdge) + (ourEdge * ourEdge * alpha1)) / (otherEdge * ourEdge);
			}
			else {
				// No such luck. Now, to use the closed form...
				// First matrix coefficients - rows are [0] [1], cols [1] [2] (diagonal matrix)
				float a[3] = { -(ourEdge * ourEdge), ourEdge * otherEdge, -(otherEdge * otherEdge) };
				// Result vector - from the solution. Think of this as the b in Ax=b.
				float b[2] = { our_edge_list_ws[i] * ourEdge - other_edge_list_ws[j] * ourEdge,
					other_edge_list_ws[j] * otherEdge - our_edge_list_ws[i] * otherEdge };

				// Closed form for a_1 and a_2... BROKEN.
				alpha2 = (b[1] - b[0] * (a[1] / a[0])) / (a[2] - a[1] * (a[1] / a[0]));
				alpha1 = (b[0] - a[1] * alpha2) / a[0];/*
													   alpha2 = (b[1] - (a[2] * b[0]) / a[0]) / (a[2] - (a[1] * a[1]) / a[0]);
													   alpha1 = (b[0] - (a[1] / (a[2] - (a[1] * a[1]) / a[0])) * (b[1] - (a[1] * b[0]) / a[0])) / a[0];*/
			}

			// Now we have our alpha values. Early out if they are out of bounds.
			if (alpha1 < 0.f || alpha1 > 1.f || alpha2 > 1.f || alpha2 < 0.f)
				continue;

			// They are within bounds. Find vector from closest points...
			Vect3 edgeDistance_ws((other_edge_list_ws[j] + otherEdge * alpha2) - (our_edge_list_ws[i] + ourEdge * alpha1));

			// Check against world space direction vector between the two boxes. If the number is positive, get out.
			if (edgeDistance_ws * direction_ws > 0.f)
				continue;
			else {
				// Houston, we have collision.

				o_list.push_back(new Contact(m_parent, b->m_parent,
					our_edge_list_ws[i] + ourEdge * alpha1,
					edgeDistance_ws.GetNormal(),
					edgeDistance_ws.Magnitude(),
					0.8f));

				contactFound = true;
			}
		}
	}

	return contactFound;
}