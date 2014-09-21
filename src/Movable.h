/////////////////////////////////////////
// Movable: Represents an object that can
//  be moved (has a position and orientation)
/////////////////////////////////////////

#ifndef FROST_MOVABLE_H
#define FROST_MOVABLE_H

#include "FrostMath.h"

namespace Frost
{
	class Movable
	{
	public:
		/////////////////// CTORS //////////////////////

		// Default ctor - origin with no orientation
		Movable();

		// Copy constructor
		Movable(const Movable& toCopy);

		// Initialize with a position, no orientation
		Movable(const FLOAT3& pos);

		// Initialize with a position and orientation
		Movable(const FLOAT3& pos, const Quaternion& orientation);

		// TODO: Add a Movable(Matrix) ctor?

		//////////////// GETTERS/SETTERS /////////////////
		virtual Vect3 GetPos() const;
		virtual Quaternion GetOrientation() const;
		virtual void SetPos(const Vect3& position);
		virtual void SetOrientation(const Quaternion& orientation);
		virtual Matrix GetTransformMatrix();
		virtual Matrix GetTransformMatrix() const;

		//////////////// TRANSFORM OPS ////////////////////
		virtual void Move(const FLOAT3& moveAmount_world);
		virtual void Rotate(const Quaternion& rotateAmount_world);

	private:
		Vect3 _pos;
		Quaternion _orientation;

		bool _isDirty;
		Matrix _transformMatrix;
	};
}

#endif