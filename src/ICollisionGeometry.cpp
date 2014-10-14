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
#include "ICollisionGeometry.h"
using namespace Frost;

ICollisionGeometry::ICollisionGeometry(FROST_COLLISION_GEOMETRY_TYPE type)
: _type(type)
, _attachedObject(0)
{}

ICollisionGeometry::ICollisionGeometry(FROST_COLLISION_GEOMETRY_TYPE type, const FLOAT3& pos, const Quaternion& orientation)
: Movable(pos, orientation)
, _type(type)
, _attachedObject(0)
{}

ICollisionGeometry::ICollisionGeometry(FROST_COLLISION_GEOMETRY_TYPE type, const FLOAT3& pos, const Quaternion& orientation, IPhysicsObject* attached)
: Movable(pos, orientation)
, _type(type)
, _attachedObject(attached)
{}

ICollisionGeometry::ICollisionGeometry(const ICollisionGeometry& copy)
: Movable(copy)
, _type(copy._type)
, _attachedObject(copy._attachedObject)
{}

FROST_COLLISION_GEOMETRY_TYPE ICollisionGeometry::getType()
{
	return _type;
}

IPhysicsObject* ICollisionGeometry::getAttachedObjectPtr() const
{
	return _attachedObject;
}

void ICollisionGeometry::attachObject(IPhysicsObject* t)
{
	if (_attachedObject == 0)
		_attachedObject = t;
	else
		throw DuplicateActionException();
	// TODO: Replace this line with a more meaningful exception
}