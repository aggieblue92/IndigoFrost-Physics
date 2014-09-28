#ifndef FROST_COLLIDABLE_INTERFACE_H
#define FROST_COLLIDABLE_INTERFACE_H

/////////////////////////////////////////
// Collidable: Represents an object with
//  collision geometry
/////////////////////////////////////////

#include "ICollisionGeometry.h"
#include "IPhysicsObject.h"
#include <vector>

namespace Frost
{
	class Collidable
	{
	public:
		Collidable();
		Collidable(IPhysicsObject* toAttach);
		~Collidable();

		void attachObject(IPhysicsObject*);

		IPhysicsObject* getAttachedObject() const;

		Matrix getTransform(int index) const;
		ICollisionGeometry* getCollisionObject(int index) const;

		void addCollisionObject(ICollisionGeometry* toAdd, const Matrix& offset);
		void removeCollisionObject(int index);

		virtual bool isTouching(Collidable* other);
		virtual void genContacts(Collidable* other, std::vector<IContact*>& o_contactList);

		bool isDirty() const;
		void clean();

		int getNumObjects() const;

	protected:
		std::vector<ICollisionGeometry*> _collisionGeometryList;
		std::vector<Matrix> _collisionGeometryTransforms;
		IPhysicsObject* _attachedObject;
		bool _isDirty;

	private:
		void updateMatrices();
	};
}

#endif