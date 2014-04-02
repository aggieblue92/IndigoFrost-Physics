#ifndef INDIGO_FROST_COLLISION_GEOMETRY
#define INDIGO_FROST_COLLISION_GEOMETRY

/*****************************************************\

	CollisionGeometry - base class for all bounding
geometry used in the bounding volume heirarchy and such.

NOTE: All types of geometry go in here. When filling
  a new type of geometry, make sure to create the
  following methods (let's call the new class newClass):

  public virtual void Geometry::fillNewType(newType* n) = 0;
  -This method fills in a zero if the object is not of
    this type, and a non-zero if it is not of the type.

  public virtual bool Geometry::isInContact(newType* n) = 0;
  -Are they colliding? If so, send the flag.

  public virtual unsigned int Geometry::generateContacts(newType* n, ContactArray* c, int remaining) = 0;
  -Adds contacts where contacts are due, if contacts are due.

\*****************************************************/

#include "Vect3.h"
#include "Matrix4.h"
#include "Quaternion.h"
#include "Collisions.h"
#include <vector>

#include <fstream>
#include <iostream>

namespace Frost {
	// LIST OF THINGS IN THIS FILE
	enum GEOMETRY_TYPE;
	class RigidBody;
	struct Contact;
	class Geometry;
	class Sphere;
	class Box;
	
	// Type of objects that this version of Indigo Frost supports
	enum GEOMETRY_TYPE {
		BOX, SPHERE
	};

	//---------------------- GEOMETRY ----------------------\\
	// Base geometry class - pure virtual,
	//  used in bounding volume hierarchies.
	class Geometry {
	protected:
		Matrix4 m_transform; // Contains position and orientation of object (ls_geometry->ls_rb)
		enum GEOMETRY_TYPE m_type; // Contains what type of geometry this is
		RigidBody* m_parent; // Contains the rigid body to which this is attached (NULL for no parent)
		Matrix4 m_transform_ws; // Contains position and orientation of object (ls_geometry->ws)

		std::ofstream* debug;

	public:
		// Base ctor - initializes to origin, no rotation, type whatever is passed.
		Geometry(RigidBody* parent, enum GEOMETRY_TYPE type);

		// Initializes to existing transform matrix
		Geometry(RigidBody* parent, const Matrix4& transform, enum GEOMETRY_TYPE type);

		// Creates a transform matrix from position and quaternion given
		Geometry(RigidBody* parent, const Vect3& position, const Quaternion& orientation, enum GEOMETRY_TYPE type);

		// Return transform matrix - transform from local space to world space
		Matrix4 getTransformMatrix() const;
		void setTransformMatrix(const Matrix4&);
		Vect3 getPosition() const;

		// Call every frame in which fine collision needs to be done on this thing
		void updateFineMatrix();

		enum GEOMETRY_TYPE getType() const;

		// Fills the given sphere with geometry data. Will make s = NULL if not a sphere.
		virtual void fillSphere(Sphere*& s) const = 0;

		// Fills the given box with geometry data. Will make s = NULL if not a box.
		virtual void fillBox(Box*& b) const = 0;

		// Generic - this goes through every type of geometry, returns if they are 
		//  touching or not. If you know what type the object is, use the isTouching(Sphere) or (Box)
		//  versions of this function.
		virtual bool genContacts(Geometry* g, std::vector<Contact*>& o_list) const;
		// Is touching - generic
		virtual bool isTouching(Geometry* g) const;

		// Returns true if this object is touching the given sphere.
		virtual bool genContacts(Sphere* s, std::vector<Contact*>& o_list) const = 0;

		// Returns true if this boject is touching the given box.
		virtual bool genContacts(Box* s, std::vector<Contact*>& o_list) const = 0;

		// virtual unsigned int GenerateContacts(Sphere* s) = 0;
		// virtual unsigned int GenerateContacts(Box* b) = 0;

		void setDebugOut(std::ofstream& out);
	};

	//---------------------- SPHERE ----------------------\\
	// Sphere class - also contains radius and stuff.
	// For coarse collision detection, use this one more.
	class Sphere : public Geometry {
	protected:
		float m_radius; // Radius of the sphere

	public:
		// Base ctor - radius 0, position 0.
		Sphere(RigidBody* parent);

		// Position and radius included.
		Sphere(RigidBody* parent, Vect3 pos, float radius);

		// Copy ctor
		Sphere(const Sphere&);

		float getRadius();
		void setRadius(float radius);

		// Fills the sphere with data from this sphere.
		virtual void fillSphere(Sphere*& s) const;

		// Fills the box with NULL, as this is not a box.
		//  In future versions, consider filling with a box that
		//  would contain all of this sphere?
		virtual void fillBox(Box*& b) const;

		// Returns true if this sphere is touching the given sphere.
		virtual bool isTouching(Sphere* s) const;
		virtual bool genContacts(Sphere* s, std::vector<Contact*>& o_list) const;

		// Returns true if this sphere is touching the given box.
		virtual bool isTouching(Box* b) const;
		virtual bool genContacts(Box* b, std::vector<Contact*>& o_list) const;

		// virtual unsigned int GenerateContacts(Sphere* s) = 0;
		// virtual unsigned int GenerateContacts(Box* b) = 0;
	};

	class Box : public Geometry {
	private:
		Vect3 m_halfSize; // Half-size in x, y, z coordinates

	public:
		// Base ctor - size 0, default geometry.
		Box(RigidBody* parent);

		// Full ctor - position, rotation, halfsize
		Box(RigidBody* parent, Vect3 pos, Quaternion rot, Vect3 halfSize);

		// Full ctor with transform matrix instead
		Box(RigidBody* parent, Matrix4 transform, Vect3 halfSize);

		// Copy ctor
		Box(const Box&);

		RigidBody* getParent();

		// Returns the half size along all 3 axis of this box
		Vect3 getHalfSize() const;
		void setHalfSize(float x, float y, float z);
		void setHalfSize(Vect3 halfSize);

		// Fills the sphere with NULL, as this is a box.
		virtual void fillSphere(Sphere*& s) const;

		// Fills the passed box with a new box containing
		//  all the information of this one.
		virtual void fillBox(Box*& b) const;

		// Returns true if this box is touching the given sphere.
		virtual bool isTouching(Sphere* s) const;
		virtual bool genContacts(Sphere* s, std::vector<Contact*>& o_list) const;

		// Returns true if this box is touching the given box.
		virtual bool isTouching(Box* b) const;
		virtual bool genContacts(Box* s, std::vector<Contact*>& o_list) const;

		// virtual unsigned int GenerateContacts(Sphere* s) = 0;
		// virtual unsigned int GenerateContacts(Box* b) = 0;
	};
}

#endif