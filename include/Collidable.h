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
#ifndef FROST_COLLIDABLE_INTERFACE_H
#define FROST_COLLIDABLE_INTERFACE_H

/////////////////////////////////////////
// Collidable: Represents an object with
//  collision geometry
/////////////////////////////////////////

#include <vector>
#include <memory>
#include <sstream>
#include "ICollisionGeometry.h"
#include "IPhysicsObject.h"

namespace Frost
{
	class Collidable
	{
	public:
		Collidable();
		Collidable(std::shared_ptr<IPhysicsObject> toAttach);
		~Collidable();

		void attachObject(std::shared_ptr<IPhysicsObject>);

		std::shared_ptr<IPhysicsObject> getAttachedObject() const;

		Matrix getTransform(int index) const;
		std::shared_ptr<ICollisionGeometry> getCollisionObject(int index) const;

		void addCollisionObject(std::shared_ptr<ICollisionGeometry> toAdd, const Matrix& offset);
		void removeCollisionObject(int index);

		virtual bool isTouching(std::shared_ptr<Collidable> other);
		virtual void genContacts(std::shared_ptr<Collidable> other, std::vector<std::shared_ptr<IContact>>& o_contactList);

		bool isDirty() const;
		void clean();

		int getNumObjects() const;

	protected:
		std::vector<std::shared_ptr<ICollisionGeometry>> _collisionGeometryList;
		std::vector<Matrix> _collisionGeometryTransforms;
		std::shared_ptr<IPhysicsObject> _attachedObject;
		bool _isDirty;

	private:
		void updateMatrices();
	};
}

#endif