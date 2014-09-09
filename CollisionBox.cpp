#include "CollisionBox.h"
#include "CollisionSphere.h"
using namespace Frost;


CollisionBox::CollisionBox(const FLOAT3& size)
: ICollisionGeometry(FROST_COLLISION_GEOMETRY_TYPE::BOX)
, _size(size)
{}

CollisionBox::CollisionBox(const FLOAT3& size, const FLOAT3& position, const Quaternion& orientation)
: ICollisionGeometry(FROST_COLLISION_GEOMETRY_TYPE::BOX, position, orientation)
, _size(size)
{}

CollisionBox::CollisionBox(const CollisionBox& o)
: ICollisionGeometry(o)
, _size(o._size)
{}

bool CollisionBox::isTouching(ICollisionGeometry* other) const
{
	if (other->GetType() == FROST_COLLISION_GEOMETRY_TYPE::BOX)
	{
		return isTouchingB((CollisionBox*)other);
	}
	else if (other->GetType() == FROST_COLLISION_GEOMETRY_TYPE::SPHERE)
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
	if (other->GetType() == FROST_COLLISION_GEOMETRY_TYPE::BOX)
	{
		genContactsB((CollisionBox*)other, o_cl);
	}
	else if (other->GetType() == FROST_COLLISION_GEOMETRY_TYPE::SPHERE)
	{
		genContactsS((CollisionSphere*)other, o_cl);
	}
	else
	{
		throw UnsupportedCollisionGeometryEvent(_type, other->GetType());
	}
}

/////////////////////////// LELs ///////////////////

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
			pt_2_ls = Vect3(-pt_1_ls._x, pt_1_ls._y, pt_2_ls._z);
			opt_2_ls = Vect3(-opt_1_ls._x, opt_1_ls._y, opt_2_ls._z);
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
			pt_2_ls = Vect3(pt_1_ls._x, -pt_1_ls._y, pt_2_ls._z);
			opt_2_ls = Vect3(opt_1_ls._x, -opt_1_ls._y, opt_2_ls._z);
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
			pt_2_ls = Vect3(pt_1_ls._x, pt_1_ls._y, -pt_2_ls._z);
			opt_2_ls = Vect3(opt_1_ls._x, opt_1_ls._y, -opt_2_ls._z);
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

void CollisionBox::VirginSacrifices(const Vect3& pt11, const Vect3& pt12, const Vect3& pt21, const Vect3& pt22, std::vector<IContact*>& o_Contacts) const
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

	// They are within bounds. Generate contact...
	o_Contacts.push_back(this->SummonDemons(
		pt11 + ourEdge * alpha1,
		(pt21 + otherEdge * alpha2) - (pt11 + ourEdge * alpha1)));
}

IContact* CollisionBox::SummonDemons(const Vect3& point, const Vect3& penetration) const
{
	// TODO: This will return a contact object.
	//  This will be called by 'Virgin Sacrifices'
	// This function is virtual to allow for derived classes to make fancier contacts.

	// return new BasicContact(point, penetration);

	return 0;
}