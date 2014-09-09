#ifndef FROSTDLL_API
#ifdef _WINDLL
#define FROSTDLL_API __declspec(dllexport)
#else
#define FROSTDLL_API __declspec(dllimport)
#endif
#endif

/////////////////////////////////////////
// ICollisionGeometry: Interface for collision
//  geometry. Encapsulates all supported types.
/////////////////////////////////////////

#include "Movable.h"
#include "IContact.h"
#include <vector>

namespace Frost
{
	enum FROST_COLLISION_GEOMETRY_TYPE
	{
		BOX, SPHERE
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
		FROSTDLL_API ICollisionGeometry(FROST_COLLISION_GEOMETRY_TYPE type);

		// Construct with type and local spatial information
		FROSTDLL_API ICollisionGeometry(FROST_COLLISION_GEOMETRY_TYPE type, const FLOAT3& pos, const Quaternion& orientation);

		// Copy ctor
		FROSTDLL_API ICollisionGeometry(const ICollisionGeometry& copy);

		/////////////////// VIRTUAL FUNCS ///////////////
		FROSTDLL_API virtual bool isTouching(ICollisionGeometry* otherGeometry) const = 0;
		FROSTDLL_API virtual void genContacts(ICollisionGeometry* otherGeometry, std::vector<IContact*>& o_list) const = 0;

		/////////////////// HELPERS //////////////////////
		FROSTDLL_API FROST_COLLISION_GEOMETRY_TYPE GetType();

	protected:
		FROST_COLLISION_GEOMETRY_TYPE _type;
	};
}