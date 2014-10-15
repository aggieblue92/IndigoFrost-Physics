#include "FrostCollisionTestSceneNode.h"

FrostCollisionTestSceneNode::FrostCollisionTestSceneNode(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id, Frost::IPhysicsNode* node)
: irr::scene::ISceneNode(parent, mgr, id)
, _frostNode(node)
, _verts(0)
, _indices(0)
{
	_material.Wireframe = false;
	_material.Lighting = false;

	auto _geo = node->getCollidableData()->getCollisionObject(0);

	// Setup the vertices here...
	if (_geo->getType() == Frost::FROST_COLLISION_GEOMETRY_TYPE_BOX)
	{
		Frost::Vect3 d = ((Frost::CollisionBox*)_geo)->getSize();

		_verts.resize(24);

		// Front face...
		_verts[0] = irr::video::S3DVertex(-d._x, -d._y, -d._z, 0.f, 0.f, -1.f, irr::video::SColor(255, 155, 0, 0), 0, 1);
		_verts[1] = irr::video::S3DVertex(-d._x, d._y, -d._z, 0.f, 0.f, -1.f, irr::video::SColor(255, 155, 0, 0), 0, 0);
		_verts[2] = irr::video::S3DVertex(d._x, d._y, -d._z, 0.f, 0.f, -1.f, irr::video::SColor(255, 155, 0, 0), 1, 0);
		_verts[3] = irr::video::S3DVertex(d._x, -d._y, -d._z, 0.f, 0.f, -1.f, irr::video::SColor(255, 155, 0, 0), 1, 1);

		// Back face...
		_verts[4] = irr::video::S3DVertex(-d._x, -d._y, d._z, 0.f, 0.f, 1.f, irr::video::SColor(255, 0, 155, 0), 0, 1);
		_verts[5] = irr::video::S3DVertex(d._x, -d._y, d._z, 0.f, 0.f, 1.f, irr::video::SColor(255, 0, 155, 0), 0, 0);
		_verts[6] = irr::video::S3DVertex(d._x, d._y, d._z, 0.f, 0.f, 1.f, irr::video::SColor(255, 0, 155, 0), 1, 0);
		_verts[7] = irr::video::S3DVertex(-d._x, d._y, d._z, 0.f, 0.f, 1.f, irr::video::SColor(255, 0, 155, 0), 1, 1);

		// Left face...
		_verts[8] = irr::video::S3DVertex(-d._x, -d._y, d._z, -1.f, 0.f, 0.f, irr::video::SColor(255, 155, 0, 155), 0, 1);
		_verts[9] = irr::video::S3DVertex(-d._x, d._y, d._z, -1.f, 0.f, 0.f, irr::video::SColor(255, 155, 0, 155), 0, 0);
		_verts[10] = irr::video::S3DVertex(-d._x, d._y, -d._z, -1.f, 0.f, 0.f, irr::video::SColor(255, 155, 0, 155), 1, 0);
		_verts[11] = irr::video::S3DVertex(-d._x, -d._y, -d._z, -1.f, 0.f, 0.f, irr::video::SColor(255, 155, 0, 155), 1, 1);

		// Right face...
		_verts[12] = irr::video::S3DVertex(d._x, -d._y, -d._z, 1.f, 0.f, 0.f, irr::video::SColor(255, 0, 155, 155), 0, 1);
		_verts[13] = irr::video::S3DVertex(d._x, d._y, -d._z, 1.f, 0.f, 0.f, irr::video::SColor(255, 0, 155, 155), 0, 0);
		_verts[14] = irr::video::S3DVertex(d._x, d._y, d._z, 1.f, 0.f, 0.f, irr::video::SColor(255, 0, 155, 155), 1, 0);
		_verts[15] = irr::video::S3DVertex(d._x, -d._y, d._z, 1.f, 0.f, 0.f, irr::video::SColor(255, 0, 155, 155), 1, 1);

		// Top face...
		_verts[16] = irr::video::S3DVertex(-d._x, d._y, -d._z, 0.f, 1.f, 0.f, irr::video::SColor(255, 0, 0, 155), 0, 1);
		_verts[17] = irr::video::S3DVertex(-d._x, d._y, d._z, 0.f, 1.f, 0.f, irr::video::SColor(255, 0, 0, 155), 0, 0);
		_verts[18] = irr::video::S3DVertex(d._x, d._y, d._z, 0.f, 1.f, 0.f, irr::video::SColor(255, 0, 0, 155), 1, 0);
		_verts[19] = irr::video::S3DVertex(d._x, d._y, -d._z, 0.f, 1.f, 0.f, irr::video::SColor(255, 0, 0, 155), 1, 1);

		// Bottom face...
		_verts[20] = irr::video::S3DVertex(-d._x, -d._y, -d._z, 0.f, -1.f, 0.f, irr::video::SColor(255, 155, 155, 0), 0, 1);
		_verts[21] = irr::video::S3DVertex(d._x, -d._y, -d._z, 0.f, -1.f, 0.f, irr::video::SColor(255, 155, 155, 0), 0, 0);
		_verts[22] = irr::video::S3DVertex(d._x, -d._y, d._z, 0.f, -1.f, 0.f, irr::video::SColor(255, 155, 155, 0), 1, 0);
		_verts[23] = irr::video::S3DVertex(-d._x, -d._y, d._z, 0.f, -1.f, 0.f, irr::video::SColor(255, 155, 155, 0), 1, 1);

		// Create the indices...
		_indices.resize(36);

		// Front
		_indices[0] = 0; _indices[1] = 1; _indices[2] = 2;
		_indices[3] = 0; _indices[4] = 2; _indices[5] = 3;

		// Back
		_indices[6] = 4; _indices[7] = 5; _indices[8] = 6;
		_indices[9] = 4; _indices[10] = 6; _indices[11] = 7;

		// Top
		_indices[12] = 8; _indices[13] = 9; _indices[14] = 10;
		_indices[15] = 8; _indices[16] = 10; _indices[17] = 11;

		// Bottom
		_indices[18] = 12; _indices[19] = 13; _indices[20] = 14;
		_indices[21] = 12; _indices[22] = 14; _indices[23] = 15;

		// Left
		_indices[24] = 16; _indices[25] = 17; _indices[26] = 18;
		_indices[27] = 16; _indices[28] = 18; _indices[29] = 19;

		// Right
		_indices[30] = 20; _indices[31] = 21; _indices[32] = 22;
		_indices[33] = 20; _indices[34] = 22; _indices[35] = 23;
	}
	else if (_geo->getType() == Frost::FROST_COLLISION_GEOMETRY_TYPE_SPHERE)
	{
		// Create vertices...
		_verts.clear();
		_indices.clear();

		float rad = ((Frost::CollisionSphere*)_geo)->getRadius();

		// Place on the top vertex...
		_verts.push_back(irr::video::S3DVertex(0.f, rad, 0.f, 0.f, 1.f, 0.f, irr::video::SColor(255, 0, 0, 155), 0, 0));

		const int STACKS = 40;
		const int RINGS = 40;
		float dPhi = Frost::MathConstants::PI / STACKS;
		float dThet = Frost::MathConstants::PI_MULT_2 / RINGS;

		// Compute vertices for each stack ring...
		for (unsigned int i = 1; i <= STACKS - 1; ++i)
		{
			float phi = i * dPhi;
			// Verts for a ring
			for (unsigned int j = 0; j <= RINGS; ++j)
			{
				float theta = j * dThet;

				_verts.push_back(irr::video::S3DVertex(
					rad * std::sin(phi) * std::cos(theta),
					rad * std::cos(phi),
					rad * std::sin(phi) * std::sin(theta),
					-rad * std::sin(phi) * std::sin(theta),
					0.f,
					rad * std::sin(phi) * std::cos(theta),
					irr::video::SColor(155, i * 155 / STACKS, 0, 155 - (i * 155 / STACKS)),
					theta / Frost::MathConstants::PI_MULT_2,
					phi / Frost::MathConstants::PI_MULT_2));
			}
		}
		// Bottom vertex...
		_verts.push_back(irr::video::S3DVertex(0.f, -rad, 0.f, 0.f, -1.f, 0.f, irr::video::SColor(255, 155, 0, 0), 0, 1));

		// Connect top pole to first ring...
		for (unsigned int i = 1u; i <= RINGS; ++i)
		{
			_indices.push_back(0);
			_indices.push_back(i + 1);
			_indices.push_back(i);
		}

		// Inner stacks...
		unsigned int base = 1u;
		unsigned int ringVertexCount = RINGS + 1u;
		for (unsigned int i = 0u; i < STACKS - 2; ++i)
		{
			for (unsigned int j = 0u; j < RINGS; ++j)
			{
				_indices.push_back(base + i * ringVertexCount + j);
				_indices.push_back(base + i * ringVertexCount + j + 1);
				_indices.push_back(base + (i + 1) * ringVertexCount + j);

				_indices.push_back(base + (i + 1) * ringVertexCount + j);
				_indices.push_back(base + i * ringVertexCount + j + 1);
				_indices.push_back(base + (i + 1) * ringVertexCount + j + 1);
			}
		}

		// Indices for bottom pole...
		unsigned int bottomIndex = (unsigned int)_verts.size() - 1u;
		base = bottomIndex - ringVertexCount;

		for (unsigned int i = 0; i < RINGS; ++i)
		{
			_indices.push_back(bottomIndex);
			_indices.push_back(base + i);
			_indices.push_back(base + i + 1);
		}
	}
	else
	{
		throw Frost::NotImplementedException();
	}

	// Finally, set up the bounding box of the node...
	_box.reset(_verts[0].Pos);
	for (unsigned int i = 1; i < _verts.size(); ++i)
	{
		_box.addInternalPoint(_verts[i].Pos);
	}
}

void FrostCollisionTestSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
	{
		SceneManager->registerNodeForRendering(this);
	}

	ISceneNode::OnRegisterSceneNode();
}

void FrostCollisionTestSceneNode::render()
{
	auto driver = SceneManager->getVideoDriver();
	driver->setMaterial(_material);
	driver->setTransform(irr::video::ETS_WORLD, AbsoluteTransformation);
	driver->drawVertexPrimitiveList(&_verts[0], _verts.size(), &_indices[0], _indices.size() / 3,
		irr::video::EVT_STANDARD, irr::scene::EPT_TRIANGLES, irr::video::EIT_32BIT);
}

void FrostCollisionTestSceneNode::updateAbsolutePosition()
{
	// Update the absolute position of this sucker...
	Frost::Vect3 axis;
	Frost::Vect3 pos;
	Frost::Quaternion orientation;
	float angle;
	_frostNode->getPhysicsObject()->getTransformMatrix().getOrientationAndPosition(pos, orientation);
	orientation.getAxisAngle(axis, angle);

	irr::core::matrix4 m, n, f;
	m.setTranslation(irr::core::vector3df(pos._x, pos._y, pos._z));
	n.setRotationAxisRadians(angle, irr::core::vector3df(axis._x, axis._y, axis._z));
	f = m * n;

	AbsoluteTransformation = f;

	//ISceneNode::updateAbsolutePosition();
}

irr::core::vector3df convFrostVect(const Frost::Vect3& o)
{
	return irr::core::vector3df(o._x, o._y, o._z);
}