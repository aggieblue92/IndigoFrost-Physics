#ifndef INDIGO_FROST_CONTACT
#define INDIGO_FROST_CONTACT

/*****************************************************\

	Contact - A structure holding
information about contacts.

contactPoint_wp: 3D position of contact (in world coords)
normal_wd: Direction of contact in world coords
penetration: Depth of interpenetration at contact point

Notice convention: wp/wd at end of var names stands for
"world point" or "world direction"

	Collision - A structure holding data
about collisions..

contacts: Array of contacts to write into.
remaining: Maximum number of contacts array can hold.

	CollisionHandler: Helper class for doing collision
things.

\*****************************************************/

#include "Vect3.h"
#include "RigidBody.h"

namespace Frost {
	//class RigidBody;
	

	//class Contact_Old {
	//public:
	//	Vect3 contactPoint_wp;
	//	Vect3 normal_wd;
	//	float penetration;

	//	// Rigid bodies held...
	//	RigidBody* rb[2];

	//	// Restitution and friction coefficients -
	//	//  in essence, bounciness and slidiness.
	//	float restitution;
	//	float friction;

	//	// Closing velocity at the point of the contact.
	//	Vect3 contactVelocity;

	//	// Required change in velocity for this contact to be resolved.
	//	float deltaVelocityRequired;

	//	// Holds world space position of the contact point relative to center of each body.
	//	Vect3 relativeContactPosition[2];
	//	void CalculateContactBasis();
	//	void CalculateInternals(float duration);
	//	Vect3 CalculateLocalVelocity(unsigned bodyIndex, float duration);

	//	void CalculateDesiredDeltaVelocity(float duration);

	//	// Perform inertia weighted penetration resolution of this contact alone.
	//	void ApplyPositionChange(Vect3 linearChange[2], Vect3 angularChange[2], float penetration);
	//	void ApplyVelocityChange(Vect3 velocityChange[2], Vect3 rotationChange[2]);
	//};

	//struct CollisionData {
	//	Contact* contactArray; // Actual contacts.
	//	Contact* contacts; // This is just for writing
	//	unsigned int remaining;
	//	unsigned int count;

	//	float restitution;
	//	float friction;
	//	float tolerance; // Uncolliding objects within a certain 'tolerance' are still colliding.

	//	void addContacts(unsigned int count);
	//};

	//class Primitive {
	//public:
	//	Vect3 getAxis(int index) const;
	//	const Matrix4& getTransform() const;

	//	RigidBody* m_rb;
	//	Matrix4 m_offset;
	//
	//protected:
	//	Matrix4 m_transform;
	//};

	//class CollisionSphere : public Primitive {
	//public:
	//	CollisionSphere();

	//	float getRadius() const;
	//	void setRadius(float newRadius);

	//private:
	//	float m_radius;
	//};

	//class CollisionPlane {
	//public:
	//	Vect3 getNormal() const;
	//	void setNormal(const Vect3& newNormal);
	//	float getOffset() const;
	//	void setOffset(float newOffset);
	//private:
	//	Vect3 m_normal;
	//	float m_offset;
	//};

	//class CollisionBox : public Primitive {
	//public:
	//	Vect3 halfSize; // Size of box on each axis.
	//	//  Total size of box along each axis is twice this value.
	//};
	//
	//class CollisionHandler {
	//public:
	//	unsigned int SphereSphere(const CollisionSphere& sphere_1, const CollisionSphere& sphere_2, CollisionData* data);
	//	unsigned int SphereHalfspace(const CollisionSphere& sphere, const CollisionPlane& halfspace, CollisionData* data);
	//	unsigned int SpherePlane(const CollisionSphere& sphere, const CollisionPlane& plane, CollisionData* data);

	//	// CAUTION: THIS ONE IS EXPENSIVE.
	//	unsigned int BoxPlane(const CollisionBox& box, const CollisionPlane& plane, CollisionData* data);
	//	unsigned int BoxSphere(const CollisionBox& box, const CollisionSphere& sphere, CollisionData* data);

	//	unsigned int BoxPoint(const CollisionBox& box, const Vect3& point, CollisionData* data);
	//	unsigned int BoxBox(const CollisionBox& box1, const CollisionBox& box2, CollisionData* data);
	//};

	//class ContactResolver {
	//public:
	//	void ResolveContacts(Contact* contactArray, unsigned int nContacts, float duration);

	//protected:
	//	void PrepareContacts(Contact* contactArray, unsigned int nContacts, float duration);
	//	void AdjustPositions(Contact* c, unsigned int num, float duration);
	//	void AdjustVelocities(Contact* c, unsigned int num, float duration);

	//	unsigned int positionIterations, rotationIterations;
	//	unsigned int usedPositionIterations, usedRotationIterations;

	//	unsigned int velocityIterations;
	//	unsigned int usedVelocityIterations;

	//	// Smallest considered "interpenetrating" amounts.
	//	float positionEpsilon, velocityEpsilon;
	//};
}
#endif
