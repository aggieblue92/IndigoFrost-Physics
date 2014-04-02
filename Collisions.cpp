#include "Collisions.h"
#include <math.h>
#include <float.h>
using namespace Frost;

static inline float transformToAxis(const CollisionBox& box, const Vect3& axis) {
	return
		box.halfSize.x * abs(axis * box.getAxis(0)) +
		box.halfSize.y * abs(axis * box.getAxis(1)) +
		box.halfSize.z * abs(axis * box.getAxis(2));
}

static inline float penetrationOnAxis(const CollisionBox& one, const CollisionBox& two, const Vect3& axis, const Vect3& toCenter) {
	// Project half-size onto one axis
	float oneProject = transformToAxis(one, axis);
	float twoProject = transformToAxis(two, axis);

	// Project onto this axis.
	float distance = abs(toCenter * axis);

	// Return overlap.
	return (oneProject + twoProject - distance);
}

static inline bool tryAxis(const CollisionBox& one, const CollisionBox& two, Vect3& axis, const Vect3& toCenter, unsigned int index,
	float& smallestPenetration, unsigned int& smallestCase)
{
	// Don't check almost parallel axis.
	if(axis.SquareMagnitude() < 0.0001f)
		return true;
	axis.Normalize();

	float penetration = penetrationOnAxis(one, two, axis, toCenter);

	if(penetration < 0.f)
		return false;
	if(penetration < smallestPenetration) {
		smallestPenetration = penetration;
		smallestCase = index;
	}

	return true;
}

static inline void fillPointFaceBoxBox(const CollisionBox& one, const CollisionBox& two,
	const Vect3& toCenter, CollisionData* data, unsigned int best, float penetration)
{
	// Method is called when we know that a vertex from box two is
	//  in contact with box one.
	Vect3 normal = one.getAxis(best);
	if(one.getAxis(best) * toCenter > 0)
		normal *= -1.f;

	// Work out which vertex of box two we're colliding with.
	Vect3 vertex = two.halfSize;
	if(two.getAxis(0) * normal < 0) vertex.x *= -1.f;
	if(two.getAxis(1) * normal < 0) vertex.y *= -1.f;
	if(two.getAxis(2) * normal < 0) vertex.z *= -1.f;

	// Create contact data
	data->contacts->normal_wd = normal;
	data->contacts->penetration = penetration;
	data->contacts->contactPoint_wp = two.getTransform() * vertex;
	data->contacts->rb[0] = one.m_rb;
	data->contacts->rb[1] = two.m_rb;
	data->contacts->friction = data->friction;
	data->contacts->restitution = data->restitution;
}

static inline Vect3 contactPoint(const Vect3& pOne, const Vect3& dOne, float oneSize,
	const Vect3& pTwo, const Vect3& dTwo, float twoSize, bool useOne)
{
	Vect3 toSt, cOne, cTwo;
	float dpStaOne, dpStaTwo, dpOneTwo, smOne, smTwo;
	float denom, mua, mub;

	smOne = dOne.SquareMagnitude();
	smTwo = dTwo.SquareMagnitude();
	dpOneTwo = dTwo * dOne;

	toSt = pOne - pTwo;
	dpStaOne = dOne * toSt;
	dpStaTwo = dTwo * toSt;

	denom = smOne * smTwo - dpOneTwo * dpOneTwo;

	// Zero denominator indicates parallel lines
	if(abs(denom) < 0.0001f)
		return useOne ? pOne : pTwo;

	mua = (dpOneTwo * dpStaTwo - smTwo * dpStaOne) / denom;
	mub = (smOne * dpStaTwo - dpOneTwo * dpStaOne) / denom;

	// If either of edges has the nearest point out of bounds, then
	//  the edges aren't crossed, we have an edge-face contact.
	// Our point is on the edge, which we know from the useOne param.
	if(mua > oneSize || mua < -oneSize || mub > twoSize || mub < -twoSize)
		return useOne ? pOne : pTwo;
	else {
		cOne = pOne + dOne * mua;
		cTwo = pTwo + dTwo * mub;

		return cOne * 0.5f + cTwo * 0.5f;
	}
}

void Contact::CalculateContactBasis() {
	Vect3 contactTangent[2];

	if(abs(normal_wd.x) > abs(normal_wd.y)) {
		// Scaling factor to ensure normalized results.
		const float scale = 1.f / sqrt(normal_wd.z * normal_wd.z + normal_wd.x * normal_wd.x);

		// New X axis is at right angles to the world Y axis.
		contactTangent[0].x = normal_wd.z * scale;
		contactTangent[0].y = 0.f;
		contactTangent[0].z = -normal_wd.x * scale;

		// New Y axis is at right angles to new X and Z axis.
		contactTangent[1].x = normal_wd.y * contactTangent[0].x;
		contactTangent[1].y = normal_wd.z * contactTangent[0].x -
			normal_wd.x * contactTangent[0].z;
		contactTangent[1].z = -normal_wd.y * contactTangent[0].x;
	} else {
		// Scaling factor to ensure normalized results.
		const float scale = 1.f / sqrt(normal_wd.z * normal_wd.z + normal_wd. y * normal_wd.y);

		// New X axis at right angles to world X axis.
		contactTangent[0].x = 0;
		contactTangent[0].y = -normal_wd.y * scale;
		contactTangent[0].z = normal_wd.y * scale;

		// New Y axis at right angles to other two axis.
		contactTangent[1].x = normal_wd.y * contactTangent[0].z -
			normal_wd.z * contactTangent[0].y;
		contactTangent[1].y = -normal_wd.x * contactTangent[0].z;
		contactTangent[1].z = normal_wd.x * contactTangent[0].y;
	}

	// Form the projection matrix from the three given basis vectors.
	contactToWorld.setComponents(
		normal_wd,
		contactTangent[0],
		contactTangent[1]);
}

void Contact::CalculateInternals(float duration) {
	// Check if first object is null, swap if it is.
	if(rb[0] == 0) {
		RigidBody* temp = rb[0];
		normal_wd *= -1.f;
		rb[0] = rb[1];
		rb[1] = temp;
	}

	// Make sure that NOW our first object isn't null.
	assert(rb[0] != 0);

	// Calculate a set of axes at the contact point.
	CalculateContactBasis();

	// Store relative position of the contact rel. to each body.
	relativeContactPosition[0] = contactPoint_wp - rb[0]->getPosition();

	if(rb[1] != 0)
	relativeContactPosition[1] = contactPoint_wp - rb[1]->getPosition();

	// Find relative velocity of the bodies at the contact point.
	//contactVelocity = calculateLocalVelocity(0, duration);
	contactVelocity = CalculateLocalVelocity(0, duration);
	if(rb[1] != 0)
		contactVelocity -= CalculateLocalVelocity(1, duration);

	// Calculate desired change in velocity for resolution.
	CalculateDesiredDeltaVelocity(duration);
}

Vect3 Contact::CalculateLocalVelocity(unsigned bodyIndex, float duration) {
	RigidBody* body = rb[bodyIndex];

	// Work out velocity of the contact point.
	Vect3 vel = Vect3::CrossProduct(body->getRotation(), relativeContactPosition[bodyIndex]);
	vel += body->getVelocity();

	// Turn velocity into contact coordinates... and return it.
	return contactToWorld.transformTranspose(vel);
}

void Contact::CalculateDesiredDeltaVelocity(float duration) {
	// Calculate acceleration-induced velocity accumulated here.
	float velocityFromAcc = 0;
	Vect3 scaledContact = normal_wd * duration;

	// Combine bounce velocity with removed acc. velocity.
	deltaVelocityRequired = -contactVelocity.x - restitution * (contactVelocity.x - velocityFromAcc);
}

void Contact::ApplyPositionChange(Vect3 linearChange[2], Vect3 angularChange[2], float penetration) {
	const float angularLimit = 0.2f;

	float angularMove[2];
	float linearMove[2];

	float totalInertia = 0.f;
	float linearInertia[2];
	float angularInertia[2];

	// Work out inertia of each object in the direction of the contact normal
	//  due only to angular inertia.
	for(unsigned int i = 0; (i < 2) && (rb[i] != 0); i++) {
		Matrix3 inverseInertiaTensor;
		inverseInertiaTensor = rb[i]->getInverseInertiaTensor();

		// Work out angular inertia.
		Vect3 angularInertiaWorld =
			Vect3::CrossProduct(relativeContactPosition[i], normal_wd);
		angularInertiaWorld =
			inverseInertiaTensor.transform(angularInertiaWorld);
		angularInertiaWorld =
			Vect3::CrossProduct(angularInertiaWorld, relativeContactPosition[i]);
		angularInertia[i] =
			angularInertiaWorld * normal_wd;

		// Linear component is simply inverse mass.
		linearInertia[i] = rb[i]->GetInverseMass();

		// Keep track of total inertia from all components.
		totalInertia += linearInertia[i] + angularInertia[i];
	}

	// Loop through again, calculating and applying changes.
	for(unsigned int i = 0; (i < 2) && (rb[i] != 0); i++) {
		// Linear and angular movements required are in proportion
		//  to the inverse inertias.
		float sign = (i == 0) ? 1 : -1;
		angularMove[i] =
			sign * penetration * (angularInertia[i] / totalInertia);
		linearMove[i] =
			sign * penetration * (linearInertia[i] / totalInertia);

		// Avoid angular projections that are too great, limit the angular move.
		Vect3 projection = relativeContactPosition[i];
		projection +=
			Vect3::DotProduct(relativeContactPosition[i] * -1, normal_wd);

		// Use small angle approximation for sine of angle...
		float maxMagnitude = angularLimit * projection.Magnitude();

		if(angularMove[i] < -maxMagnitude) {
			float totalMove = angularMove[i] + linearMove[i];
			angularMove[i] = -maxMagnitude;
			linearMove[i] = totalMove - angularMove[i];
		}
		else if(angularMove[i] > maxMagnitude) {
			float totalMove = angularMove[i] + linearMove[i];
			angularMove[i] = maxMagnitude;
			linearMove[i] = totalMove - angularMove[i];
		}

		// We have linear movement... Now calculate desired rotation to achieve that.
		if(angularMove[i] == 0) {
			// No angular movement? No rotation.
			angularChange[i] = Vect3(0.f, 0.f, 0.f);
		} else {
			// Well, work out direction of rotation..
			Vect3 targetAngularDirection =
				Vect3::CrossProduct(relativeContactPosition[i], normal_wd);

			Matrix3 inverseInertiaTensor =
				rb[i]->getInverseInertiaTensorWorld();

			// Work out direction we'd need to rotate to achieve that.
			angularChange[i] =
				inverseInertiaTensor.transform(targetAngularDirection) *
				(angularMove[i] / angularInertia[i]);
		}

		// Velocity change is easier - just linear movement along normal.
		linearChange[i] = normal_wd * linearMove[i];

		// Now, start to apply values we've calculated...
		Vect3 pos;
		pos = rb[i]->getPosition();
		pos += (normal_wd * linearMove[i]);
		rb[i]->setPosition(pos);

		Quaternion q;
		q = rb[i]->getOrientation();
		q.addScaledVector(angularChange[i], 1.f);
		rb[i]->setOrientation(q);

		// Finally, calculate derived data.
		rb[i]->calculateDerivedData();
	}
}

void Contact::ApplyVelocityChange(Vect3 velocityChange[2], Vect3 rotationChange[2]) {
	// Get hold of inverse mass and tensors, both in world coords.
	Matrix3 inverseInertiaTensor[2];
	inverseInertiaTensor[0] = rb[0]->getInverseInertiaTensorWorld();
	if(rb[1] != 0)
		inverseInertiaTensor[1] = rb[1]->getInverseInertiaTensorWorld();

	// Calculate impulse for each contact axis...
	Vect3 impulseContact;

	// For now, use short format.
	impulseContact /*=calculateFrictionLEssImpulse(inverseInertiaTensor)*/;

	Vect3 impulse = contactToWorld.transform(impulseContact);

	// Split impulse into linear and rotation components.
	Vect3 impulsiveTorque = Vect3::CrossProduct(relativeContactPosition[0], impulse);
	rotationChange[0] = inverseInertiaTensor[0].transform(impulsiveTorque);
	velocityChange[0] = Vect3(0.f, 0.f, 0.f);
	velocityChange[0] += impulse * rb[0]->GetInverseMass();

	// Apply changes.
	// CLEANUP for better form.
	rb[0]->m_velocity += velocityChange[0];
	rb[0]->m_rotation += rotationChange[0];

	if(rb[1] != 0) {
		// Work out linear and angular changes, much as before.
		impulsiveTorque = Vect3::CrossProduct(impulse, relativeContactPosition[1]);
		rotationChange[1] = inverseInertiaTensor[1].transform(impulsiveTorque);
		velocityChange[1] = Vect3(0.f, 0.f, 0.f);
		velocityChange[1] += impulse * (-rb[0]->GetInverseMass());

		// Apply. CLEANUP FOR BETTER FORM.
		rb[1]->m_velocity += velocityChange[1];
		rb[1]->m_rotation += rotationChange[1];
	}
}

void CollisionData::addContacts(unsigned int count) {
	this->remaining -= count;
	this->contacts += count;
}

Vect3 Primitive::getAxis(int index) const {
	return m_offset.getAxisVector(index);
}

const Matrix4& Primitive::getTransform() const {
	return m_transform;
}

float CollisionSphere::getRadius() const {
	return this->m_radius;
}

Vect3 CollisionPlane::getNormal() const {
	return m_normal;
}

void CollisionPlane::setNormal(const Vect3& newNormal) {
	if(!(newNormal.x == 0.f && newNormal.y == 0.f && newNormal.z == 0.f))
		m_normal = newNormal;
}

float CollisionPlane::getOffset() const {
	return m_offset;
}

void CollisionPlane::setOffset(float newOffset) {
	m_offset = newOffset;
}

unsigned int CollisionHandler::SphereSphere(const CollisionSphere& s1,  const CollisionSphere& s2, CollisionData* dat) {
	// Make sure we have contacts...
	if(dat->remaining <= 0)
		return 0;

	// Cache the sphere positions.
	Vect3 s1Pos = s1.getAxis(3);
	Vect3 s2Pos = s2.getAxis(3);

	// Find the vector between the objects...
	Vect3 midline = s1Pos - s2Pos;
	float len = midline.Magnitude();

	// See if it is large enough....
	if(len <= 0.f || len >= s1.getRadius() + s2.getRadius())
		return 0;

	// Create the normal (manually)
	Vect3 normal = midline * (1.f / len);

	Contact* contact = dat->contacts;
	contact->normal_wd = normal;
	contact->contactPoint_wp = s1Pos + midline * 0.5f;
	contact->penetration = (s1.getRadius() + s2.getRadius() - len);

	// Write appropriate data
	contact->rb[0] = s1.m_rb;
	contact->rb[1] = s2.m_rb;
	contact->restitution = dat->restitution;
	contact->friction = dat->friction;

	dat->addContacts(1);
	return 1;
}

unsigned int CollisionHandler::SphereHalfspace(const CollisionSphere& sphere, const CollisionPlane& halfspace, CollisionData* data) {
	// Check for contacts...
	if(data->remaining <= 0.f)
		return 0;

	// Cache sphere position
	Vect3 position = sphere.getAxis(3);

	// Find distance from the plane
	float distance = halfspace.getNormal() * position - sphere.getRadius() - halfspace.getOffset();

	if (distance >= 0.f)
		return 0;

	// Create contact
	Contact* contact = data->contacts;
	contact->normal_wd = halfspace.getNormal();
	contact->penetration = -distance;
	contact->contactPoint_wp = position - halfspace.getNormal() * (distance + sphere.getRadius());

	contact->rb[0] = sphere.m_rb;
	contact->rb[1] = 0;
	contact->restitution = data->restitution;
	contact->friction = data->friction;

	data->addContacts(1);
	return 1;
}

unsigned int CollisionHandler::SpherePlane(const CollisionSphere& sphere, const CollisionPlane& plane, CollisionData* data) {
	// Make sure we have contacts left to fill...
	if(data->remaining <= 0)
		return 0;

	// Cache sphere position
	Vect3 pos = sphere.getAxis(3);

	// Find distance from plane.
	float distance = plane.getNormal() * pos - sphere.getRadius() - plane.getOffset();

	if(distance * distance > sphere.getRadius() * sphere.getRadius())
		return 0;

	// Check which side of the plane we're on.
	Vect3 normal = plane.getNormal();
	float penetration = -distance;
	if(distance < 0.f) {
		normal *= -1;
		penetration *= -1;
	}
	penetration += sphere.getRadius(); // Wouldn't this go inside the if statement?

	// Create the contact... Normal in plane direction
	Contact* contact = data->contacts;
	contact->normal_wd = normal;
	contact->penetration = penetration;
	contact->contactPoint_wp =
		pos - plane.getNormal() * (distance); // Also not sure about this.

	// Write appropriate data
	contact->rb[0] = sphere.m_rb;
	contact->rb[1] = 0;
	contact->restitution = data->restitution;
	contact->friction = data->friction;

	data->addContacts(1);
	return 1;
}

unsigned int CollisionHandler::BoxPlane(const CollisionBox& box, const CollisionPlane& plane, CollisionData* data) {

	// Early out if we're out of collisions...
	if (data->remaining <= 0)
		return 0;

	// Early out if the box and space can't be colliding..
	// -Projected radius of box onto the plane direction
	// -Work out how far the box is from the origin
	// -Check for intersection
	float projectedRadius = transformToAxis(box, plane.getNormal());
	float boxDistance = plane.getNormal() * box.getAxis(3) - projectedRadius;
	if(boxDistance > plane.getOffset())
		return 0;

	Contact* contact = data->contacts;
	unsigned int contactsUsed = 0;

	Vect3 vertexPos[8] =
	{
		box.m_offset * Vect3(-box.halfSize.x, -box.halfSize.y, -box.halfSize.z),
		box.m_offset * Vect3(box.halfSize.x, -box.halfSize.y, -box.halfSize.z),
		box.m_offset * Vect3(-box.halfSize.x, box.halfSize.y, -box.halfSize.z),
		box.m_offset * Vect3(box.halfSize.x, box.halfSize.y, -box.halfSize.z),
		box.m_offset * Vect3(-box.halfSize.x, -box.halfSize.y, box.halfSize.z),
		box.m_offset * Vect3(box.halfSize.x, -box.halfSize.y, box.halfSize.z),
		box.m_offset * Vect3(-box.halfSize.x, box.halfSize.y, box.halfSize.z),
		box.m_offset * Vect3(box.halfSize.x, box.halfSize.y, box.halfSize.z)
	};

	// CODE TO GENERATE CONTACT AT ONE VERTEX.
	// Find distance from plane, compare to plane distance.
	for(unsigned int i = 0; i < 8; i++) {
		float vertexDistance = vertexPos[i] * plane.getNormal();
		if(vertexDistance <= plane.getOffset()) {
			// Create contact data 
			//  -Contact Point: halfway between vertex and plane.
			contact->contactPoint_wp = plane.getNormal();
			contact->contactPoint_wp *= (vertexDistance - plane.getOffset());
			contact->contactPoint_wp += vertexPos[i];
			contact->normal_wd = plane.getNormal();
			contact->penetration = plane.getOffset() - vertexDistance;

			// Write appropriate data
			contact->rb[0] = box.m_rb;
			contact->rb[1] = 0;
			contact->friction = data->friction;
			contact->restitution = data->restitution;

			// Move onto the next contact
			contact++;
			contactsUsed++;
			if(contactsUsed >= data->remaining)
				return contactsUsed;
		}
	}

	data->remaining -= contactsUsed;
	return contactsUsed;
}

unsigned int CollisionHandler::BoxSphere(const CollisionBox& box, const CollisionSphere& sphere, CollisionData* data) {
	// Transform center of sphere into box coordinates...
	Vect3 sphere_origin = sphere.getAxis(3);
	Vect3 sphere_origin_blp = box.getTransform().transformInverse(sphere_origin);

	// Early out - see if we can exclude the contact because
	//  the sphere is outside on all 3 sets of axis.
	if(abs(sphere_origin_blp.x) - sphere.getRadius() > box.halfSize.x ||
		abs(sphere_origin_blp.y) - sphere.getRadius() > box.halfSize.y ||
		abs(sphere_origin_blp.z) - sphere.getRadius() > box.halfSize.z)
		return 0;

	// Clamp and test each coordinate of the box.
	Vect3 closestPoint(0.f, 0.f, 0.f);
	float distance;

	distance = sphere_origin_blp.x;
	if (distance > box.halfSize.x) distance = box.halfSize.x;
	if (distance < -box.halfSize.x) distance = -box.halfSize.x;
	closestPoint.x = distance;

	distance = sphere_origin_blp.y;
	if (distance > box.halfSize.y) distance = box.halfSize.y;
	if (distance < -box.halfSize.y) distance = -box.halfSize.y;
	closestPoint.y = distance;

	distance = sphere_origin_blp.z;
	if (distance > box.halfSize.z) distance = box.halfSize.z;
	if (distance < -box.halfSize.z) distance = -box.halfSize.z;
	closestPoint.z = distance;

	// Check if we're contacting...
	distance = (closestPoint - sphere_origin_blp).SquareMagnitude();
	if (distance > sphere.getRadius() * sphere.getRadius())
		return 0;

	// Compile the contact
	Vect3 closestPoint_wp = box.getTransform().transform(closestPoint);

	Contact* contact = data->contacts;
	contact->normal_wd = (closestPoint_wp - sphere_origin);
	contact->normal_wd.Normalize();
	contact->contactPoint_wp = closestPoint_wp;
	contact->penetration = sphere.getRadius() - sqrt(distance);
	contact->rb[0] = box.m_rb;
	contact->rb[1] = sphere.m_rb;
	contact->friction = data->friction;
	contact->restitution = data->restitution;

	data->addContacts(1);
	return 1;
}

unsigned int CollisionHandler::BoxPoint(const CollisionBox& box, const Vect3& point, CollisionData* data) {
	if(data->remaining <= 0)
		return 0;

	Vect3 relPt = box.getTransform().transformInverse(point);

	Vect3 normal;

	// Check each axis, looking for the axis on which the penetration
	//  is the least deep.
	float min_depth = box.halfSize.x - abs(relPt.x);
	if (min_depth < 0)
		return 0;
	normal = box.getAxis(0) * ((relPt.x < 0.f) ? -1.f : 1.f);

	float depth = box.halfSize.y - abs(relPt.y);
	if (depth < 0)
		return 0;
	else if (depth < min_depth) {
		min_depth = depth;
		normal = box.getAxis(1) * ((relPt.y < 0.f) ? -1.f : 1.f);
	}

	depth = box.halfSize.z - abs(relPt.z);
	if(depth < 0)
		return 0;
	else if (depth < min_depth) {
		min_depth = depth;
		normal = box.getAxis(2) * ((relPt.z < 0.f) ? -1.f : 1.f);
	}

	// Create the contact
	data->contacts->normal_wd = normal;
	data->contacts->contactPoint_wp = point;
	data->contacts->penetration = min_depth;
	data->contacts->rb[0] = box.m_rb;
	data->contacts->rb[1] = 0;
	data->contacts->friction = data->friction;
	data->contacts->restitution = data->restitution;

	data->addContacts(1);
	return 1;
}

#define CHECK_OVERLAP(axis, index) \
	if (!tryAxis(box1, box2, (axis), toCenter, (index), pen, best)) return 0;
unsigned int CollisionHandler::BoxBox(const CollisionBox& box1, const CollisionBox& box2, CollisionData* data) {
	if(data->remaining <= 0)
		return 0;

	Vect3 toCenter = box2.getAxis(3) - box1.getAxis(3);

	// Start by assuming there is no contact
	float pen = FLT_MAX;
	unsigned int best = 0xffffff;

	// Check each axis, returning if it gives us
	//  a separating axis, and keeping track of the axis
	//  with the smallest penetration otherwise.
	CHECK_OVERLAP(box1.getAxis(0), 0);
	CHECK_OVERLAP(box1.getAxis(1), 1);
	CHECK_OVERLAP(box1.getAxis(2), 2);

	CHECK_OVERLAP(box1.getAxis(0), 3);
	CHECK_OVERLAP(box1.getAxis(1), 4);
	CHECK_OVERLAP(box1.getAxis(2), 5);

	// Store the best axis-major, in case we run into almost parallel
	//  edge collisions later
	unsigned int bestSingleAxis = best;

	CHECK_OVERLAP(Vect3::CrossProduct(box1.getAxis(0), box2.getAxis(0)), 6);
	CHECK_OVERLAP(Vect3::CrossProduct(box1.getAxis(0), box2.getAxis(1)), 7);
	CHECK_OVERLAP(Vect3::CrossProduct(box1.getAxis(0), box2.getAxis(2)), 8);
	CHECK_OVERLAP(Vect3::CrossProduct(box1.getAxis(1), box2.getAxis(0)), 9);
	CHECK_OVERLAP(Vect3::CrossProduct(box1.getAxis(1), box2.getAxis(1)), 10);
	CHECK_OVERLAP(Vect3::CrossProduct(box1.getAxis(1), box2.getAxis(2)), 11);
	CHECK_OVERLAP(Vect3::CrossProduct(box1.getAxis(2), box2.getAxis(0)), 12);
	CHECK_OVERLAP(Vect3::CrossProduct(box1.getAxis(2), box2.getAxis(1)), 13);
	CHECK_OVERLAP(Vect3::CrossProduct(box1.getAxis(2), box2.getAxis(2)), 14);

	// Make sure we've got a result...
	assert(best != 0xffffff);

	// We know there's a collision if we're here.
	//  We also know which asix gave the smallest penetration.
	//  We now can deal with it depending on the case.
	if(best < 3) {
		// Vertex of box two on face of box one.
		fillPointFaceBoxBox(box1, box2, toCenter, data, best, pen);
		data->addContacts(1);
		return 1;
	} else if (best < 6) {
		// Vertex of box one on face of box two.
		fillPointFaceBoxBox(box2, box1, toCenter * -1.f, data, best - 3, pen);
		data->addContacts(1);
		return 1;
	} else {
		// (1-epsilon)% chance we've got an edge-edge contact.
		best -= 6;
		unsigned int oneAxisIndex = best / 3;
		unsigned int twoAxisIndex = best % 3;
		Vect3 oneAxis = box1.getAxis(oneAxisIndex);
		Vect3 twoAxis = box2.getAxis(twoAxisIndex);
		Vect3 axis = Vect3::CrossProduct(oneAxis, twoAxis);
		axis.Normalize();

		// The axis should point from box one to box two.
		if(axis * toCenter > 0.f)
			axis *= -1.f;

		// We have the axis, but not the edges. Each axis has 4 edges
		//  parallel to it, we need to find which of the 4 for each object. We do that
		//  by finding the point in the center of the edge. We know its component in the
		//  direction of the box's collision axis is zero, and we determine which of the
		//  extremes in each of the other axis is closest.
		Vect3 ptOnOneEdge = box1.halfSize;
		Vect3 ptOnTwoEdge = box2.halfSize;
		for(unsigned int i = 0; i < 3; i++) {
			// Potential error - in if statements, boxn.getAxis(i)... > or < 0? Which?)
			if(i == oneAxisIndex)
				ptOnOneEdge[i] = 0;
			else if(box1.getAxis(i) * axis > 0)
				ptOnOneEdge[i] = -ptOnOneEdge[i];

			if(i == twoAxisIndex)
				ptOnTwoEdge[i] = 0;
			else if(box2.getAxis(i) * axis < 0)
				ptOnTwoEdge[i] = -ptOnTwoEdge[i];
		}

		// Move them into world coordinates. They're already oriented correctly.
		ptOnOneEdge = box1.getTransform() * ptOnOneEdge;
		ptOnTwoEdge = box2.getTransform() * ptOnTwoEdge;

		// We have a point and direction.
		//  We need to find point of closest approach.
		Vect3 vertex = contactPoint(ptOnOneEdge,
			oneAxis, box1.halfSize[oneAxisIndex], ptOnTwoEdge,
			twoAxis, box2.halfSize[twoAxisIndex],
			bestSingleAxis > 2);

		// Fill the contact.
		data->contacts->penetration = pen;
		data->contacts->normal_wd = axis;
		data->contacts->contactPoint_wp = vertex;
		data->contacts->rb[0] = box1.m_rb;
		data->contacts->rb[1] = box2.m_rb;
		data->contacts->friction = data->friction;
		data->contacts->restitution = data->restitution;
		data->addContacts(1);
		return 1;
	}

	return 0;
}
#undef CHECK_OVERLAP

void ContactResolver::ResolveContacts(Contact* contactsArray, unsigned int nContacts, float duration) {
	// Early out if we are out of things to do.
	if(nContacts <= 0)
		return;

	// Prepare contacts for processing.
	PrepareContacts(contactsArray, nContacts, duration);

	// Resolve interpenetration, and then velocity issues.
	AdjustPositions(contactsArray, nContacts, duration);
	// adjustVelocities(contacts, nContacts, duration);
}

void ContactResolver::PrepareContacts(Contact* contactArray, unsigned int nContacts, float duration) {
	// Generate contact velocity and axis information...
	Contact* last = contactArray + nContacts;
	for(Contact* contact = contactArray; contact < last; contact++) {
		contact->CalculateInternals(duration);
	}
}

void ContactResolver::AdjustPositions(Contact* c, unsigned int num, float duration) {
	unsigned int index, i;
	Vect3 velocityChange[2], rotationChange[2];
	float max;
	Vect3 deltaPosition;

	// Iteratively resolve interpenetration in order of severity.
	usedPositionIterations = 0;
	while(usedPositionIterations < positionIterations) {
		max = positionEpsilon;
		index = num;
		for(unsigned int i = 0; i < num; i++) {
			if(c[i].penetration > max) {
				max = c[i].penetration;
				index = i;
			}
		}

		if(index == num) break;

		// Resolve penetration
		c[index].ApplyPositionChange(velocityChange, rotationChange, max);
		
		// This may have changed penetration of others, so update contacts...
		// SWEET JESUS THIS IS A TRIPLE FOR LOOP.
		for (unsigned int i = 0; i < num; i++) {
			// Check each body in the contact
			for (unsigned int b = 0; (b < 2) && (c[i].rb[b] != 0); b++) {
				// Check for a match with each body in the newly
				// resolved contact
				for (unsigned int d = 0; d < 2; d++) {
					if (c[i].rb[b] == c[index].rb[d]) {
						deltaPosition = velocityChange[d] +
							Vect3::CrossProduct(rotationChange[d],
							c[i].relativeContactPosition[b]);
						// The sign of the change is positive if we're
						// dealing with the second body in a contact
						// and negative otherwise (because we're
						// subtracting the resolution)..
						c[i].penetration +=
							Vect3::DotProduct(deltaPosition, c[i].normal_wd) * (b ? 1 : -1);
					} // Dios
				} // mio
			} // que
		} // FEO!
		usedPositionIterations++;
	} // Seriously. Gross.
}

void ContactResolver::AdjustVelocities(Contact* c, unsigned int num, float duration) {
	Vect3 velocityChange[2], rotationChange[2];
	Vect3 deltaVelocity;

	// Iteratively handle impacts in order of severity.
	velocityIterations = 0;
	while (usedVelocityIterations < velocityIterations) {
		// Find contact with maximum magnitude of velocity change.
		float max = velocityEpsilon;
		unsigned int index = num;
		for(unsigned int i = 0; i < num; i++) {
			if(c[i].deltaVelocityRequired > max) {
				max = c[i].deltaVelocityRequired;
				index = i;
			}
		}
		if(index == num)
			break;

		// Do resolution on contact that came out on top.
		c[index].ApplyVelocityChange(velocityChange, rotationChange);

		// With change in velocity of the two bidies, the update
		//  of contact velocities means some need recomputing.
		for(unsigned int i = 0; i < num; i++) {
			// Check each body in contact...
			for(unsigned int b = 0; b < 2 && (c[i].rb[b] != 0); b++) {
				// Check for a match with each body in newly resolved contact.
				for(unsigned int d = 0; d < 2 && (c[index].rb[d] != 0); d++) {
					if(c[i].rb[b] == c[index].rb[d]) {
						deltaVelocity = velocityChange[d] +
							Vect3::CrossProduct(rotationChange[d],
							c[i].relativeContactPosition[b]);

						// Sign of change is negative if second body.
						c[i].contactVelocity +=
							c[i].contactToWorld.transformTranspose(deltaVelocity) * (b ? -1 : 1);
						c[i].CalculateDesiredDeltaVelocity(duration);
					}
				}
			}
		}
		usedVelocityIterations++;
	}
}