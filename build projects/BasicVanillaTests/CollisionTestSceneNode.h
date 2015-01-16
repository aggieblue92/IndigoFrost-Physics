#ifndef FROST_IRR_TEST_SCENE_NODE
#define FROST_IRR_TEST_SCENE_NODE

#include <FrostWorldMan.h>
#include <irrlicht.h>
#include <vector>

namespace FrostTest
{
	class CollisionSceneNode : public irr::scene::ISceneNode
	{
	public:
		CollisionSceneNode(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 idm, std::shared_ptr<Frost::IPhysicsNode> physicsNode);

		virtual void OnRegisterSceneNode();

		virtual void render();

		virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const { return _box; }
		virtual irr::u32 getMaterialCount() const { return 1; }
		virtual irr::video::SMaterial& getMaterial(irr::u32 index) { return _material; }

		virtual void updateAbsolutePosition();

	private:
		irr::core::aabbox3d<irr::f32> _box;

		std::vector<irr::video::S3DVertex> _verts;
		std::vector<irr::u32> _indices;
		irr::video::SMaterial _material;

		std::shared_ptr<Frost::IPhysicsNode> _frostNode;
	};

	irr::core::vector3df convFrostVect(const Frost::Vect3& o);
}

#endif
