#include <irrlicht.h>
#include <driverChoice.h>
#include <IndigoFrost.h>
#include "DemoTimer.h"
#include "CollisionTestSceneNode.h"
#include "DemoInput.h"
#include <sstream>

// CONSTANTS for DEMO
const int BALL_CT = 15;
const float PLAYER_ACC_TIME = 1.5f;
const float MAX_SPEED = 200.f;

class StuffBrokeException { public: std::string errMessage; StuffBrokeException(std::string m) : errMessage(m) {} };

void SetupDevice(irr::IrrlichtDevice*& o_device, irr::IEventReceiver& er)
{
	// Ask user for driver type, create device...
	auto driverType = irr::driverChoiceConsole();
	if (irr::video::EDT_COUNT == driverType)
	{
		throw StuffBrokeException("Could not initialize driver of specified type!");
	}
	o_device = irr::createDevice(driverType, irr::core::dimension2d<irr::u32>(800, 600), 32, false, false, false, &er);

	if (!o_device)
	{
		throw StuffBrokeException("Could not create device!");
	}

	// Create engine and camera...
	o_device->setWindowCaption(L"RollingOnTable Demo - Indigo Frost demo (powered by Irrlicht)");
	auto driver = o_device->getVideoDriver();
	auto smgr = o_device->getSceneManager();
	smgr->addCameraSceneNodeFPS();
}

void SetupNodes(irr::IrrlichtDevice* device, Frost::WorldManager& root, const std::vector<std::string>& namesList)
{
	auto smgr = device->getSceneManager();
	for (unsigned int i = 0u; i < namesList.size(); ++i)
	{
		FrostTest::CollisionSceneNode* myNode = new FrostTest::CollisionSceneNode(smgr->getRootSceneNode(), smgr, 678, root[namesList[i]]);
		myNode->drop();
		myNode = 0;
	}
}

void UpdatePlayerMotionForce(DemoInput& dip, std::shared_ptr<Frost::PlayerMotionForce> pmf)
{
	Frost::Vect3 dirn(Frost::MathConstants::VECTOR_ZERO);
	if (dip.IsKeyDown(irr::KEY_KEY_W))
	{
		dirn += Frost::MathConstants::VECTOR_UNIT_Z;
	}
	if (dip.IsKeyDown(irr::KEY_KEY_S))
	{
		dirn -= Frost::MathConstants::VECTOR_UNIT_Z;
	}
	if (dip.IsKeyDown(irr::KEY_KEY_D))
	{
		dirn += Frost::MathConstants::VECTOR_UNIT_X;
	}
	if (dip.IsKeyDown(irr::KEY_KEY_A))
	{
		dirn -= Frost::MathConstants::VECTOR_UNIT_X;
	}

	pmf->setDirectionAndSpeed(dirn, MAX_SPEED);
}

int main()
{
	// Set up Irrlicht...
	irr::IrrlichtDevice* device = nullptr;
	DemoInput dip;
	SetupDevice(device, dip);

	// <PHYSICS>
	Frost::WorldManager root(Frost::FROST_COLLISION_MANAGER_BRUTE_FORCE);

	// Keep registry of named objects
	std::vector<std::string> objNames;

	// Create objects...
	std::stringstream ss("");
	for (int i = 0; i < BALL_CT; ++i)
	{
		ss << "Ball" << i;
		objNames.push_back(ss.str());
		root.addObject(std::shared_ptr<Frost::BasicPhysicsObject>(new Frost::BasicPhysicsObject(10.f, Frost::MathConstants::MATRIX_IDENTITY,
			0.08f, 0.08f, Frost::Vect3(80.f * std::sin(i  * Frost::MathConstants::PI_MULT_2 / BALL_CT),
			80.f,
			400.f + 120.f * std::cos(i * Frost::MathConstants::PI_MULT_2 / BALL_CT)))), ss.str());
		root[ss.str()]->getCollidableData()->addCollisionObject(
			std::shared_ptr<Frost::CommonCollisionSphere>(new Frost::CommonCollisionSphere(10.f,
			root[ss.str()]->getPhysicsObject()->getPos(), 0.3f, 10.f, root[ss.str()]->getPhysicsObject())), Frost::MathConstants::MATRIX_IDENTITY);
		root.addForce(std::make_shared<Frost::GravityForce>(Frost::MathConstants::VECTOR_UNIT_Y * -1.f, 98.1f), ss.str());
	}

	std::shared_ptr<Frost::PlayerMotionForce> playerMotionForce = std::make_shared<Frost::PlayerMotionForce>(Frost::MathConstants::VECTOR_ZERO, PLAYER_ACC_TIME);
	root.addForce(playerMotionForce, "Ball0");

	root.addObject(std::make_shared<Frost::BasicPhysicsObject>(0.f, Frost::MathConstants::MATRIX_ZERO,
		1.f, 1.f, Frost::Vect3(0.f, -300.f, 400.f)),
		"Table");
	objNames.push_back("Table");
	root["Table"]->getCollidableData()->addCollisionObject(
		std::make_shared<Frost::CommonCollisionBox>(Frost::Vect3(400.f, 10.f, 400.f), root["Table"]->getPhysicsObject()->getPos(),
		Frost::MathConstants::QUATERNION_UNIT, 0.f, 0.f, root["Table"]->getPhysicsObject()), Frost::MathConstants::MATRIX_IDENTITY);

	// </PHYSICS>

	// Create our scene nodes...
	SetupNodes(device, root, objNames);

	// Main game loop
	irr::u32 frames = 0;
	DemoTimer timer;
	auto driver = device->getVideoDriver();
	auto smgr = device->getSceneManager();
	while (device->run())
	{
		timer.tick();
		root.update(timer.getFrameTime());

		// Update our PlayerMotionForce

		driver->beginScene(true, true, irr::video::SColor(0, 100, 100, 100));
		smgr->drawAll();
		driver->endScene();

		if (++frames == 100)
		{
			irr::core::stringw str = L"Indigo Frost demo - Irrlicht [";
			str += timer.getFrameTime();
			str += L" ms";
			str += L"] FPS: ";
			str += (irr::s32)driver->getFPS();

			UpdatePlayerMotionForce(dip, playerMotionForce);

			device->setWindowCaption(str.c_str());
			frames = 0;
		}
	}

	device->drop();
	device = 0;

	return 0;
}