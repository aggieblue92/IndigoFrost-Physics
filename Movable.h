#ifndef FROSTDLL_API
#ifdef _WINDLL
#define FROSTDLL_API __declspec(dllexport)
#else
#define FROSTDLL_API __declspec(dllimport)
#endif
#endif

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
		FROSTDLL_API Movable();

		// Copy constructor
		FROSTDLL_API Movable(const Movable& toCopy);

		// Initialize with a position, no orientation
		FROSTDLL_API Movable(const FLOAT3& pos);

		// Initialize with a position and orientation
		FROSTDLL_API Movable(const FLOAT3& pos, const Quaternion& orientation);

		// TODO: Add a Movable(Matrix) ctor?

		//////////////// GETTERS/SETTERS /////////////////
		FROSTDLL_API virtual Vect3 GetPos() const;
		FROSTDLL_API virtual Quaternion GetOrientation() const;
		FROSTDLL_API virtual void SetPos(const Vect3& position);
		FROSTDLL_API virtual void SetOrientation(const Quaternion& orientation);
		FROSTDLL_API virtual Matrix GetTransformMatrix() const;

		//////////////// TRANSFORM OPS ////////////////////
		FROSTDLL_API virtual void Move(const FLOAT3& moveAmount_world);
		FROSTDLL_API virtual void Rotate(const Quaternion& rotateAmount_world);

	private:
		Vect3 _pos;
		Quaternion _orientation;
	};
}

#endif