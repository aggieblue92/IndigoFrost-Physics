#include <irrlicht.h>
#include "FrostCollisionTestSceneNode.h"
#include "DemoTimer.h"
#include "DemoInput.h"
#include <FrostWorldMan.h>
#include <driverChoice.h>
#include <sstream>

const float PLAYER_ACC_TIME = 1.5f;
const float MAX_SPEED = 200.f;

class StuffBroke { public: std::string errMessage; StuffBroke(std::string msg) : errMessage(msg) {} };

void setUpBoringStuff(irr::IrrlichtDevice*& o_device, irr::IEventReceiver& er)
{
	// Ask user for driver type and create the device...
	auto driverType = irr::driverChoiceConsole();
	//irr::video::E_DRIVER_TYPE driverType = irr::video::EDT_DIRECT3D9;
	if (driverType == irr::video::EDT_COUNT)
	{
		throw StuffBroke("Could not initialize driver of specified type!");
	}
	o_device = irr::createDevice(driverType, irr::core::dimension2d<irr::u32>(800, 600), 32, false, false, false, &er);

	if (o_device == 0)
	{
		throw StuffBroke("Could not create device!");
	}

	// Create engine and camera...
	o_device->setWindowCaption(L"Custom Scene Node - Irrlicht Engine Demo");
	auto driver = o_device->getVideoDriver();
	auto smgr = o_device->getSceneManager();
	smgr->addCameraSceneNodeFPS();
}

void setUpBoringStuff(irr::IrrlichtDevice* device, Frost::WorldManager& root, const std::vector<std::string>& namesList)
{
	auto smgr = device->getSceneManager();

	for (unsigned int i = 0; i < namesList.size(); ++i)
	{
		FrostCollisionTestSceneNode* myNode = new FrostCollisionTestSceneNode(smgr->getRootSceneNode(), smgr, 666, root[namesList[i]]);
		myNode->drop();
		myNode = 0;
	}
}

int main()
{
	// Set up Irrlicht...
	irr::IrrlichtDevice* device = 0;
	MyEventReceiver mer;
	setUpBoringStuff(device, mer);

	// This is where you set up the physics code:
	// Create the WorldManager object, using a BVH Tree for the collision manager
	const Frost::FROST_COLLISION_MANAGER collManType = Frost::FROST_COLLISION_MANAGER_BRUTE_FORCE;

	// Setup logging...
	Frost::DebugLogger::createLogger(Frost::DebugLogger::DEBUG_LEVEL_DEBUG_TO_FILE | Frost::DebugLogger::DEBUG_LEVEL_LOG_TO_CLOG | Frost::DebugLogger::DEBUG_LEVEL_LOG_TO_FILE | Frost::DebugLogger::DEBUG_LEVEL_ERR_TO_FILE | Frost::DebugLogger::DEBUG_LEVEL_ERR_TO_CERR, "log.txt");

	Frost::WorldManager root(collManType);


	// We need to keep a registry of the named objects we are using - otherwise,
	//  we (currently) have no way of accessing them.
	std::vector<std::string> objNames;

	// Add objects to the scene...
	const int CT = 15;
	for (int i = 0; i < CT; ++i)
	{
		std::stringstream ss("");
		ss << "Ball" << i;
		objNames.push_back(ss.str());
		root.addObject(std::shared_ptr<Frost::BasicPhysicsObject>(new Frost::BasicPhysicsObject(10.f, Frost::MathConstants::MATRIX_IDENTITY,
			0.08f, 0.08f,
			Frost::Vect3(0.f + 80.f * std::sin(i * Frost::MathConstants::PI_MULT_2 / CT),
			80.f,
			400.f + 120.f * std::cos(i * Frost::MathConstants::PI_MULT_2 / CT)))),
			ss.str());
		root[ss.str()]->getCollidableData()->addCollisionObject(
			std::shared_ptr<Frost::CommonCollisionSphere>(new Frost::CommonCollisionSphere(
			10.f, root[ss.str()]->getPhysicsObject()->getPos(), 0.3f, 10.f, root[ss.str()]->getPhysicsObject()))
			, Frost::MathConstants::MATRIX_IDENTITY);
		root.addForce(std::make_shared<Frost::GravityForce>(Frost::MathConstants::VECTOR_UNIT_Y * -1.f, 98.1f), ss.str());
	}

	std::shared_ptr<Frost::PlayerMotionForce> pmf = std::shared_ptr<Frost::PlayerMotionForce>(new Frost::PlayerMotionForce(Frost::MathConstants::VECTOR_ZERO, PLAYER_ACC_TIME));
	root.addForce(pmf, "Ball0");

	root.addObject(std::make_shared<Frost::BasicPhysicsObject>(0.f, Frost::MathConstants::MATRIX_ZERO,
		1.f, 1.f, Frost::Vect3(0.f, -300.f, 400.f)),
		"Table");
	objNames.push_back("Table");

	// Add collision information to the objects in our scene
	root["Table"]->getCollidableData()->addCollisionObject(
		std::make_shared<Frost::CommonCollisionBox>(Frost::Vect3(400.f, 10.f, 400.f), root["Table"]->getPhysicsObject()->getPos(),
		Frost::MathConstants::QUATERNION_UNIT, 0.f, 0.f, root["Table"]->getPhysicsObject()),
		Frost::MathConstants::MATRIX_IDENTITY);

	// Add forces to the objects in our scene

	// Create our scene nodes...
	setUpBoringStuff(device, root, objNames);

	// Main game loop...
	irr::u32 frames = 0;
	DemoTimer timer;
	auto driver = device->getVideoDriver();
	auto smgr = device->getSceneManager();
	while (device->run())
	{
		timer.tick();
		root.update(timer.getFrameTime() / 1.f);

		// Update our PlayerMotionForce
		Frost::Vect3 dirn(0.f, 0.f, 0.f);
		if (mer.IsKeyDown(irr::KEY_UP))
		{
			dirn += Frost::MathConstants::VECTOR_UNIT_Z;
		}
		if (mer.IsKeyDown(irr::KEY_DOWN))
		{
			dirn -= Frost::MathConstants::VECTOR_UNIT_Z;
		}
		if (mer.IsKeyDown(irr::KEY_RIGHT))
		{
			dirn += Frost::MathConstants::VECTOR_UNIT_X;
		}
		if (mer.IsKeyDown(irr::KEY_LEFT))
		{
			dirn -= Frost::MathConstants::VECTOR_UNIT_X;
		}
		pmf->setDirectionAndSpeed(dirn, MAX_SPEED);

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

			device->setWindowCaption(str.c_str());
			frames = 0;
		}
	}

	device->drop();
	device = 0;
}
