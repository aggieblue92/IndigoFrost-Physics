/////////////////////////////////////////
// ICollisionGeometry: Interface for collision
//  geometry. Encapsulates all supported types.
/////////////////////////////////////////

#ifndef FROST_COLLISION_GEOMETRY_INTERFACE_H
#define FROST_COLLISION_GEOMETRY_INTERFACE_H

#include "Movable.h"
#include "IContact.h"
#include "IPhysicsObject.h"
#include <vector>

namespace Frost
{
	enum FROST_COLLISION_GEOMETRY_TYPE
	{
		FROST_COLLISION_GEOMETRY_TYPE_BOX, 
		FROST_COLLISION_GEOMETRY_TYPE_SPHERE
	};

	class CollisionBox;
	class CollisionSphere;

	// Exception for handling unsupported future geometry types
	class UnsupportedCollisionGeometryEvent : public NotImplementedException
	{
	public:
		FROST_COLLISION_GEOMETRY_TYPE _type1;
		FROST_COLLISION_GEOMETRY_TYPE _type2;
		UnsupportedCollisionGeometryEvent(FROST_COLLISION_GEOMETRY_TYPE firstType, FROST_COLLISION_GEOMETRY_TYPE secondType)
			: _type1(firstType), _type2(secondType) {}
	};

	class ICollisionGeometry : public Movable
	{
	public:
		/////////////////// CTORS //////////////////////

		// Base ctor - type only
		ICollisionGeometry(FROST_COLLISION_GEOMETRY_TYPE type);

		// Construct with type and local spatial information
		ICollisionGeometry(FROST_COLLISION_GEOMETRY_TYPE type, const FLOAT3& pos, const Quaternion& orientation);

		// Constructor with type, spatial information and attached object
		ICollisionGeometry(FROST_COLLISION_GEOMETRY_TYPE type, const FLOAT3& pos, const Quaternion& orientation, IPhysicsObject* attachedObject);

		// Copy ctor
		ICollisionGeometry(const ICollisionGeometry& copy);

		/////////////////// VIRTUAL FUNCS ///////////////
		virtual bool isTouching(ICollisionGeometry* otherGeometry) const = 0;
		virtual void genContacts(ICollisionGeometry* otherGeometry, std::vector<IContact*>& o_list) const = 0;

		/////////////////// HELPERS //////////////////////
		FROST_COLLISION_GEOMETRY_TYPE GetType();
		IPhysicsObject* GetAttachedObjectPtr() const;

		// Attach an object to the collision geometry. Fails if
		//  the collision geometry already has an attached object.
		void AttachObject(IPhysicsObject* toAttach);

	protected:
		FROST_COLLISION_GEOMETRY_TYPE _type;
		IPhysicsObject* _attachedObject;
	};
}

#endif