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
		FROST_COLLISION_GEOMETRY_TYPE getType();
		IPhysicsObject* getAttachedObjectPtr() const;

		// Attach an object to the collision geometry. Fails if
		//  the collision geometry already has an attached object.
		void attachObject(IPhysicsObject* toAttach);

	protected:
		FROST_COLLISION_GEOMETRY_TYPE _type;
		IPhysicsObject* _attachedObject;
	};
}

#endif