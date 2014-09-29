#include "CollisionBox.h"
#include "CollisionSphere.h"
using namespace Frost;

#define CLAMP(x, min, max)	x < min ? min : (x > max ? max : x)

CollisionBox::CollisionBox(const FLOAT3& size)
: ICollisionGeometry(FROST_COLLISION_GEOMETRY_TYPE_BOX)
, _size(size)
{}

CollisionBox::CollisionBox(const FLOAT3& size, const FLOAT3& position, const Quaternion& orientation)
: ICollisionGeometry(FROST_COLLISION_GEOMETRY_TYPE_BOX, position, orientation)
, _size(size)
{}

CollisionBox::CollisionBox(const FLOAT3& size, const FLOAT3& position, const Quaternion& orientation, IPhysicsObject* toAttach)
: ICollisionGeometry(FROST_COLLISION_GEOMETRY_TYPE_BOX, position, orientation, toAttach)
, _size(size)
{}

CollisionBox::CollisionBox(const CollisionBox& o)
: ICollisionGeometry(o)
, _size(o._size)
{}

bool CollisionBox::isTouching(ICollisionGeometry* other) const
{
	if (other->GetType() == FROST_COLLISION_GEOMETRY_TYPE_BOX)
	{
		return isTouchingB((CollisionBox*)other);
	}
	else if (other->GetType() == FROST_COLLISION_GEOMETRY_TYPE_SPHERE)
	{
		return isTouchingS((CollisionSphere*)other);
	}
	else
	{
		throw UnsupportedCollisionGeometryEvent(_type, other->GetType());
	}
}

void CollisionBox::genContacts(ICollisionGeometry* other, std::vector<IContact*>& o_cl) const
{
	if (other->GetType() == FROST_COLLISION_GEOMETRY_TYPE_BOX)
	{
		genContactsB((CollisionBox*)other, o_cl);
	}
	else if (other->GetType() == FROST_COLLISION_GEOMETRY_TYPE_SPHERE)
	{
		genContactsS((CollisionSphere*)other, o_cl);
	}
	else
	{
		throw UnsupportedCollisionGeometryEvent(_type, other->GetType());
	}
}

bool CollisionBox::isTouchingB(CollisionBox* b) const
{
	// Two types of detection to test for: edge-edge and corner-face
	// All other types will resolve to one of those two types, or can be
	//  estimated as a collection of the others.

	// Get the world position of the two boxes...
	Vect3 myPos_ws = this->GetPos();
	Vect3 otherPos_ws = b->GetPos();
	Vect3Normal dirn_ws = otherPos_ws - myPos_ws;

	// TODO: Can be optimized by running a bunch of this
	//  code across multiple threads.

	////////////////// POINT-FACE DETECTION ////////////////

	// Gather qualifying points...
	std::vector<Vect3> myQualifying_ws(0);
	std::vector<Vect3> otherQualifying_ws(0);

	this->BlackMagic(this->GetTransformMatrix().GetInverse().TransformDirn(dirn_ws), myQualifying_ws);
	b->BlackMagic(b->GetTransformMatrix().GetInverse().TransformDirn(dirn_ws), otherQualifying_ws);

	// Check qualifying points...
	for (int i = 0; i < (int)myQualifying_ws.size(); i++)
	{
		Vect3 checkPoint = b->GetTransformMatrix().GetInverse() * myQualifying_ws[i];
		if ((fabs(checkPoint._x) <= b->getSize()._x) &&
			(fabs(checkPoint._y) <= b->getSize()._y) &&
			(fabs(checkPoint._z) <= b->getSize()._z))
		{
			// Collision at a point-face, point-edge, or point-point
			return true;
		}
	}

	for (int i = 0; i < (int)otherQualifying_ws.size(); i++)
	{
		Vect3 checkPoint = this->GetTransformMatrix().GetInverse() * otherQualifying_ws[i];
		if ((fabs(checkPoint._x) <= this->getSize()._x) &&
			(fabs(checkPoint._y) <= this->getSize()._y) &&
			(fabs(checkPoint._z) <= this->getSize()._z))
		{
			// Collision at a point-face, point-edge, or point-point
			return true;
		}
	}

	////////////////// EDGE-EDGE DETECTION ////////////////

	// Gather our edges...
	std::vector<Vect3> our_edge_list_ws(0);
	std::vector<Vect3> other_edge_list_ws(0);
	BlackMagic(b, our_edge_list_ws, other_edge_list_ws);

	// Check each edge against each other edge.
	for (int i = 0; i < (int)our_edge_list_ws.size(); i += 2)
	{
		for (int j = 0; j < (int)other_edge_list_ws.size(); j += 2)
		{
			if (VirginSacrifices(dirn_ws, our_edge_list_ws[i], our_edge_list_ws[i + 1], other_edge_list_ws[j], other_edge_list_ws[j + 1], b->GetAttachedObjectPtr()))
				return true;
		}
	}

	return false;
}

void CollisionBox::genContactsB(CollisionBox* b, std::vector<IContact*>& o) const
{
	// Two types of detection to test for: edge-edge and corner-face
	// All other types will resolve to one of those two types, or can be
	//  estimated as a collection of the others.

	// Get the world position of the two boxes...
	Vect3 myPos_ws = this->GetPos();
	Vect3 otherPos_ws = b->GetPos();
	Vect3Normal dirn_ws = otherPos_ws - myPos_ws;

	// TODO: Can be optimized by running a bunch of this
	//  code across multiple threads.

	////////////////// POINT-FACE DETECTION ////////////////

	// Gather qualifying points...
	std::vector<Vect3> myQualifying_ws(0);
	std::vector<Vect3> otherQualifying_ws(0);

	this->BlackMagic(this->GetTransformMatrix().GetInverse().TransformDirn(dirn_ws), myQualifying_ws);
	b->BlackMagic(b->GetTransformMatrix().GetInverse().TransformDirn(dirn_ws), otherQualifying_ws);

	// Check qualifying points...
	for (int i = 0; i < (int)myQualifying_ws.size(); i++)
	{
		Vect3 checkPoint = b->GetTransformMatrix().GetInverse() * myQualifying_ws[i];
		if ((fabs(checkPoint._x) <= b->getSize()._x) &&
			(fabs(checkPoint._y) <= b->getSize()._y) &&
			(fabs(checkPoint._z) <= b->getSize()._z))
		{
			// Collision at a point-face, point-edge, or point-point
			//  Collision vector is direction to closest face.
			Vect3 collision = Vect3(
				(checkPoint._x > 0.f) ? b->getSize()._x - checkPoint._x : checkPoint._x + b->getSize()._x,
				(checkPoint._y > 0.f) ? b->getSize()._y - checkPoint._y : checkPoint._y + b->getSize()._y,
				(checkPoint._z > 0.f) ? b->getSize()._z - checkPoint._z : checkPoint._z + b->getSize()._z
				);

			// We'll choose direction of least penetration
			Vect3 collPt = b->GetTransformMatrix() * checkPoint;
			Vect3 collDirn = ComponentProduct(
				b->GetTransformMatrix().TransformDirn(collision),
				(collision._x < collision._y && collision._x < collision._z) ? MathConstants::VECTOR_UNIT_X
					: (collision._y < collision._z) ? MathConstants::VECTOR_UNIT_Y : MathConstants::VECTOR_UNIT_Z);
			if (this->GetAttachedObjectPtr())
			{
				// Or is it b->getattached?
				try
				{
					o.push_back(new BasicContact(collPt, collDirn, this->GetAttachedObjectPtr()));
				}
				catch (const ZeroMagnitudeException&)
				{
					// Instead push back a different kind of thing.
				}
			}
			if (b->GetAttachedObjectPtr())
			{
				try
				{
					o.push_back(new BasicContact(collPt, collDirn * -1.f, b->GetAttachedObjectPtr()));
				}
				catch (const ZeroMagnitudeException&)
				{
					// Instead push back a different kind of thing.
				}
			}
		}
	}

	for (int i = 0; i < (int)otherQualifying_ws.size(); i++)
	{
		Vect3 checkPoint = this->GetTransformMatrix().GetInverse() * otherQualifying_ws[i];
		if ((fabs(checkPoint._x) <= this->getSize()._x) &&
			(fabs(checkPoint._y) <= this->getSize()._y) &&
			(fabs(checkPoint._z) <= this->getSize()._z))
		{
			// Collision at a point-face, point-edge, or point-point
			//  Collision vector is direction to closest face.
			Vect3 collision = Vect3(
				(checkPoint._x > 0.f) ? this->getSize()._x - checkPoint._x : checkPoint._x + this->getSize()._x,
				(checkPoint._y > 0.f) ? this->getSize()._y - checkPoint._y : checkPoint._y + this->getSize()._y,
				(checkPoint._z > 0.f) ? this->getSize()._z - checkPoint._z : checkPoint._z + this->getSize()._z
				);

			// We'll choose direction of least penetration
			Vect3 collPt = this->GetTransformMatrix() * checkPoint;
			Vect3 collDirn = ComponentProduct(
				this->GetTransformMatrix().TransformDirn(collision),
				(collision._x < collision._y && collision._x < collision._z) ? MathConstants::VECTOR_UNIT_X
					: (collision._y < collision._z) ? MathConstants::VECTOR_UNIT_Y : MathConstants::VECTOR_UNIT_Z);
			if (this->GetAttachedObjectPtr())
			{
				// Or is it this->getattached?
				try
				{
					o.push_back(new BasicContact(collPt, collDirn * -1.f, this->GetAttachedObjectPtr()));
				}
				catch (const ZeroMagnitudeException&)
				{
					// Instead push back a different kind of thing.
				}
			}
			if (b->GetAttachedObjectPtr())
			{
				try
				{
					o.push_back(new BasicContact(collPt, collDirn, b->GetAttachedObjectPtr()));
				}
				catch (const ZeroMagnitudeException&)
				{
					// Instead push back a different kind of contact (bumpcontact, whatever)
				}
			}
		}
	}

	////////////////// EDGE-EDGE DETECTION ////////////////

	// Gather our edges...
	std::vector<Vect3> our_edge_list_ws(0);
	std::vector<Vect3> other_edge_list_ws(0);
	BlackMagic(b, our_edge_list_ws, other_edge_list_ws);

	// Check each edge against each other edge.
	for (int i = 0; i < (int)our_edge_list_ws.size(); i += 2)
	{
		for (int j = 0; j < (int)other_edge_list_ws.size(); j += 2)
		{
			VirginSacrifices(dirn_ws, our_edge_list_ws[i], our_edge_list_ws[i + 1], other_edge_list_ws[j], other_edge_list_ws[j + 1], o, b->GetAttachedObjectPtr());
		}
	}
}

bool CollisionBox::isTouchingS(CollisionSphere* s) const
{
	// Get the sphere position in local coordinates of the box.
	Vect3 transformedSpherePosition = this->GetTransformMatrix().GetInverse() * s->GetPos();

	// If the sphere is further away than the magnitude of the furthest distance a corner can
	//  be, get an early out:
	if (transformedSpherePosition.SquareMagnitude() > _size.SquareMagnitude()) return false;

	// Otherwise, find the closest point on the cube.
	//  For any axis, it will be the cube half-length on that
	//  side if the sphere's position is further than the half-length,
	//  or it will be the component of the sphere's location on that axis if not.
	Vect3 closestBoxPoint = transformedSpherePosition;
	closestBoxPoint._x = CLAMP(closestBoxPoint._x, -_size._x, _size._x);
	closestBoxPoint._y = CLAMP(closestBoxPoint._y, -_size._y, _size._y);
	closestBoxPoint._z = CLAMP(closestBoxPoint._z, -_size._z, _size._z);

	// Now we have the closest point on the box.
	//  If it's closer than the radius, we have contact.
	return ((closestBoxPoint - transformedSpherePosition).SquareMagnitude() <= s->GetRadius() * s->GetRadius());
}

void CollisionBox::genContactsS(CollisionSphere* s, std::vector<IContact*>& o) const
{
	// Get the sphere position in local coordinates of the box.
	Vect3 transformedSpherePosition = this->GetTransformMatrix().GetInverse() * s->GetPos();

	// If the sphere is further away than the magnitude of the furthest distance a corner can
	//  be, get an early out:
	if (transformedSpherePosition.SquareMagnitude() > _size.SquareMagnitude()) return;

	// Otherwise, find the closest point on the cube.
	//  For any axis, it will be the cube half-length on that
	//  side if the sphere's position is further than the half-length,
	//  or it will be the component of the sphere's location on that axis if not.
	Vect3 closestBoxPoint = transformedSpherePosition;
	closestBoxPoint._x = CLAMP(closestBoxPoint._x, -_size._x, _size._x);
	closestBoxPoint._y = CLAMP(closestBoxPoint._y, -_size._y, _size._y);
	closestBoxPoint._z = CLAMP(closestBoxPoint._z, -_size._z, _size._z);

	// Now we have the closest point on the box.
	//  If it's closer than the radius, we have contact.
	if ((closestBoxPoint - transformedSpherePosition).SquareMagnitude() <= s->GetRadius() * s->GetRadius())
	{
		// Box contact data: contact is under the surface of the sphere, pointing directly out.
		Vect3Normal d = (closestBoxPoint - transformedSpherePosition);
		Vect3 collisionPoint_l = transformedSpherePosition + d * s->GetRadius();
		Vect3 penetration_l = closestBoxPoint - collisionPoint_l;
		Vect3 collisionPoint_w = GetTransformMatrix() * collisionPoint_l;
		Vect3 penetration_w = GetTransformMatrix().TransformDirn(penetration_l);
			o.push_back(SummonDemons(
				collisionPoint_w,
				penetration_w,
				_attachedObject,
				s->GetAttachedObjectPtr()));

		// Sphere contact data: Exact opposite of the box contact data
		penetration_w *= -1.f;
		o.push_back(SummonDemons(
			collisionPoint_w,
			penetration_w,
			s->GetAttachedObjectPtr(),
			_attachedObject));
	}
}

/////////////////// GETTERS / SETTERS /////////////////
Vect3 CollisionBox::getSize() const
{
	return _size;
}

/////////////////////////// LELs ///////////////////

void CollisionBox::BlackMagic(const Vect3Normal& dirn_ls, std::vector<Vect3>& o_edgeList) const
{
	for (int i = 0; i < 8; i++)
	{
		Vect3 check_ls = Vect3(
			(i % 2 == 0 ? -1.f : 1.f) * _size._x,
			((i / 2) % 2 == 0 ? -1.f : 1.f) * _size._y,
			((i / 4) % 2 == 0 ? -1.f : 1.f) * _size._z);

		if (check_ls * dirn_ls > 0.f)
		{
			o_edgeList.push_back(this->GetTransformMatrix() * check_ls);
		}
	}
}

void CollisionBox::BlackMagic(CollisionBox* other, std::vector<Vect3>& o_MyEdges, std::vector<Vect3>& o_OtherEdges) const
{
	Vect3 dirn_ws = other->GetPos() - this->GetPos();

	// 12 edges: Pick two directions, the edge will be formed by the two vertices on either.
	//  e.g., pick ++, edge is +++/++-
	// Pick all edges in WORLD space
	for (int i = 0; i < 12; i++)
	{
		Vect3 pt_1_ls, opt_1_ls;
		Vect3 pt_2_ls, opt_2_ls;
		if (i / 4 == 0)
		{
			// 0-3: Variable x
			pt_1_ls = Vect3(
				_size._x,
				((i / 2) % 2 == 0 ? -1.f : 1.f) * _size._y,
				(i % 2 == 0 ? -1.f : 1.f) * _size._z);
			opt_1_ls = Vect3(
				other->_size._x,
				((i / 2) % 2 == 0 ? -1.f : 1.f) * other->_size._y,
				(i % 2 == 0 ? -1.f : 1.f) * other->_size._z);
			pt_2_ls = Vect3(-pt_1_ls._x, pt_1_ls._y, pt_1_ls._z);
			opt_2_ls = Vect3(-opt_1_ls._x, opt_1_ls._y, opt_1_ls._z);
		}
		else if (i / 4 == 1)
		{
			// 4-7: Variable y
			pt_1_ls = Vect3(
				((i / 2) % 2 == 0 ? -1.f : 1.f) * _size._x,
				_size._y,
				(i % 2 == 0 ? -1.f : 1.f) * _size._z);
			opt_1_ls = Vect3(
				((i / 2) % 2 == 0 ? -1.f : 1.f) * other->_size._x,
				other->_size._y,
				(i % 2 == 0 ? -1.f : 1.f) * other->_size._z);
			pt_2_ls = Vect3(pt_1_ls._x, -pt_1_ls._y, pt_1_ls._z);
			opt_2_ls = Vect3(opt_1_ls._x, -opt_1_ls._y, opt_1_ls._z);
		}
		else
		{
			// 8-11: Variable z
			pt_1_ls = Vect3(
				((i / 2) % 2 == 0 ? -1.f : 1.f) * _size._x,
				(i % 2 == 0 ? -1.f : 1.f) * _size._y,
				_size._z);
			opt_1_ls = Vect3(
				((i / 2) % 2 == 0 ? -1.f : 1.f) * other->_size._x,
				(i % 2 == 0 ? -1.f : 1.f) * other->_size._y,
				other->_size._z);
			pt_2_ls = Vect3(pt_1_ls._x, pt_1_ls._y, -pt_1_ls._z);
			opt_2_ls = Vect3(opt_1_ls._x, opt_1_ls._y, -opt_1_ls._z);
		}

		// Local edges are selected. If both points are in direction of other cube,
		//  add them to the edge list.
		Vect3 dirn_ls = this->GetTransformMatrix().GetInverse().TransformDirn(dirn_ws);
		if (pt_1_ls * dirn_ls > 0.f &&
			pt_2_ls * dirn_ls > 0.f)
		{
			o_MyEdges.push_back(this->GetTransformMatrix() * pt_1_ls);
			o_MyEdges.push_back(this->GetTransformMatrix() * pt_2_ls);
		}
		dirn_ls = other->GetTransformMatrix().GetInverse().TransformDirn(dirn_ws * -1.f);
		if (opt_1_ls * dirn_ls > 0.f &&
			opt_2_ls * dirn_ls > 0.f)
		{
			o_OtherEdges.push_back(other->GetTransformMatrix() * opt_1_ls);
			o_OtherEdges.push_back(other->GetTransformMatrix() * opt_2_ls);
		}
	}
}

void CollisionBox::VirginSacrifices(const Vect3Normal& dirn_ws, const Vect3& pt11, const Vect3& pt12, const Vect3& pt21, const Vect3& pt22, std::vector<IContact*>& o_Contacts, IPhysicsObject* otherObject) const
{
	// Find the edge vectors...
	Vect3 ourEdge = pt12 - pt11;
	Vect3 otherEdge = pt22 - pt21;

	float alpha1, alpha2; // Coefficients in finding the shortest distance between the edges

	// Linear algebra time! If some point ourMiddleishPoint = ourEdgeOrigin_ws + (alpha)(ourEdge),
	//  and likewise for the otherMiddleishPoint, then (otherMiddleish - ourMiddleish) * ourEdge = 0
	//  and (otherMiddleish - ourMiddleish) * otherEdge = 0, then the vector
	//  distanceVector = (otherMiddleish - ourMiddleish) is the vector from the closest point on our edge
	//  to the closest point on the other edge. This is 0 at contact, and it is in the negative direction_ws
	//  direction if the edges are interpenetrating.

	// Doing math and solving for a 2x2 system of equations, you get this:
	/*
	[[ -r_1 * r_1    r_2 * r_1 ]] [[ a_1 ]]   =   [[ p_11 * r_1 - p_21 * r_1 ]]
	[[ r_1 * r_2    -r_2 * r_2 ]] [[ a_2 ]]       [[ p_21 * r_2 - p_11 * r_2 ]]
	r_1 := ourEdge                  a_1 := percentage of length of r_1 down which the closest point to the other edge is.
	r_2 := otherEdge                a_2 := percentage of length of r_2 down which the closest point to our edge is.
	p_11 := our_edge_list_ws[i]     p_21 := other_edge_list_ws[i]
	p_12 := our_edge_list_ws[i+1]   p_22 := other_edge_list_ws[i+1]
	*/
	// Solve the system (I found a closed form using Gauss' method).

	// First off, check the case where the lines are parallel - i.e., if the first
	//  matrix is singular. In this case, just pick any a_1, solve for a_2.
	if (ourEdge * ourEdge + otherEdge * otherEdge == ourEdge * otherEdge * 2.f)
	{
		alpha1 = 0.5f;
		alpha2 = (((pt11 - pt21) * ourEdge) + (ourEdge * ourEdge * alpha1)) / (otherEdge * ourEdge);
	}
	else
	{
		// No such luck. Now to use the closed form...
		// Matrix coefficients - rows are [0] [1], cols [1] [2] (diagonal matrix)
		float a[3] = { -(ourEdge * ourEdge), ourEdge * otherEdge, -(otherEdge * otherEdge) };
		float b[2] = { (pt11 - pt21) * ourEdge, (pt21 - pt11) * otherEdge };

		// Closed form for a_1 and a_2... POSSIBLY BROKEN?
		alpha2 = (b[1] - b[0] * (a[1] / a[0])) / (a[2] - a[1] * (a[1] / a[0]));
		alpha1 = (b[0] - a[1] * alpha2) / a[0];
	}

	// Now we have our alpha values... Early out if they are out of bounds.
	if (alpha1 < 0.f || alpha1 > 1.f || alpha2 > 1.f || alpha2 < 0.f) return;

	Vect3 edgeDistance_ws((pt21 + otherEdge * alpha2) - (pt11 + ourEdge * alpha1));
	if (edgeDistance_ws * dirn_ws > 0.f) return;

	// They are within bounds. Generate contact...
	o_Contacts.push_back(this->SummonDemons(
		pt11 + ourEdge * alpha1,
		(pt11 + ourEdge * alpha1) - (pt21 + otherEdge * alpha2),
		_attachedObject, otherObject));

	o_Contacts.push_back(this->SummonDemons(
		pt21 + otherEdge * alpha2,
		(pt21 + otherEdge * alpha2) - (pt11 + ourEdge * alpha1),
		otherObject, _attachedObject));
}

bool CollisionBox::VirginSacrifices(const Vect3Normal& dirn_ws, const Vect3& pt11, const Vect3& pt12, const Vect3& pt21, const Vect3& pt22, IPhysicsObject* otherObject) const
{
	// Find the edge vectors...
	Vect3 ourEdge = pt12 - pt11;
	Vect3 otherEdge = pt22 - pt21;

	float alpha1, alpha2; // Coefficients in finding the shortest distance between the edges

	// Linear algebra time! If some point ourMiddleishPoint = ourEdgeOrigin_ws + (alpha)(ourEdge),
	//  and likewise for the otherMiddleishPoint, then (otherMiddleish - ourMiddleish) * ourEdge = 0
	//  and (otherMiddleish - ourMiddleish) * otherEdge = 0, then the vector
	//  distanceVector = (otherMiddleish - ourMiddleish) is the vector from the closest point on our edge
	//  to the closest point on the other edge. This is 0 at contact, and it is in the negative direction_ws
	//  direction if the edges are interpenetrating.

	// Doing math and solving for a 2x2 system of equations, you get this:
	/*
	[[ -r_1 * r_1    r_2 * r_1 ]] [[ a_1 ]]   =   [[ p_11 * r_1 - p_21 * r_1 ]]
	[[ r_1 * r_2    -r_2 * r_2 ]] [[ a_2 ]]       [[ p_21 * r_2 - p_11 * r_2 ]]
	r_1 := ourEdge                  a_1 := percentage of length of r_1 down which the closest point to the other edge is.
	r_2 := otherEdge                a_2 := percentage of length of r_2 down which the closest point to our edge is.
	p_11 := our_edge_list_ws[i]     p_21 := other_edge_list_ws[i]
	p_12 := our_edge_list_ws[i+1]   p_22 := other_edge_list_ws[i+1]
	*/
	// Solve the system (I found a closed form using Gauss' method).

	// First off, check the case where the lines are parallel - i.e., if the first
	//  matrix is singular. In this case, just pick any a_1, solve for a_2.
	if (ourEdge * ourEdge + otherEdge * otherEdge == ourEdge * otherEdge * 2.f)
	{
		alpha1 = 0.5f;
		alpha2 = (((pt11 - pt21) * ourEdge) + (ourEdge * ourEdge * alpha1)) / (otherEdge * ourEdge);
	}
	else
	{
		// No such luck. Now to use the closed form...
		// Matrix coefficients - rows are [0] [1], cols [1] [2] (diagonal matrix)
		float a[3] = { -(ourEdge * ourEdge), ourEdge * otherEdge, -(otherEdge * otherEdge) };
		float b[2] = { (pt11 - pt21) * ourEdge, (pt21 - pt11) * otherEdge };

		// Closed form for a_1 and a_2... POSSIBLY BROKEN?
		alpha2 = (b[1] - b[0] * (a[1] / a[0])) / (a[2] - a[1] * (a[1] / a[0]));
		alpha1 = (b[0] - a[1] * alpha2) / a[0];
	}

	// Now we have our alpha values... Early out if they are out of bounds.
	if (alpha1 < 0.f || alpha1 > 1.f || alpha2 > 1.f || alpha2 < 0.f)
		return false;

	Vect3 edgeDistance_ws((pt21 + otherEdge * alpha2) - (pt11 + ourEdge * alpha1));
	if (edgeDistance_ws * dirn_ws > 0.f)
		return false;

	return true;
}

IContact* CollisionBox::SummonDemons(const Vect3& point, const Vect3& penetration, IPhysicsObject* afObj, IPhysicsObject* oobj) const
{
	// Return a contact object.
	//  This will be called by 'Virgin Sacrifices'
	// This function is virtual to allow for derived classes to make fancier contacts.
	return new BasicContact(point, penetration, afObj);
}