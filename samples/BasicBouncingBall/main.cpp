#include <irrlicht.h>
#include "FrostCollisionTestSceneNode.h"
#include "DemoTimer.h"
#include <FrostWorldMan.h>
#include <driverChoice.h>

class StuffBroke { public: std::string errMessage; StuffBroke(std::string msg) : errMessage(msg) {} };

void setUpBoringStuff(irr::IrrlichtDevice*& o_device)
{
	// Ask user for driver type and create the device...
	auto driverType = irr::driverChoiceConsole();
	if (driverType == irr::video::EDT_COUNT)
	{
		throw StuffBroke("Could not initialize driver of specified type!");
	}
	o_device = irr::createDevice(driverType, irr::core::dimension2d<irr::u32>(800, 600), 32, false);

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
	setUpBoringStuff(device);

	// Setup the physics debugger...
	Frost::DebugLogger::createLogger(Frost::DebugLogger::DEBUG_LEVEL_DEBUG_TO_FILE | Frost::DebugLogger::DEBUG_LEVEL_LOG_TO_CLOG | Frost::DebugLogger::DEBUG_LEVEL_LOG_TO_FILE | Frost::DebugLogger::DEBUG_LEVEL_ERR_TO_FILE | Frost::DebugLogger::DEBUG_LEVEL_ERR_TO_CERR, "log.txt");

	// This is where you set up the physics code:
	// Create the WorldManager object, using a BVH Tree for the collision manager
	Frost::WorldManager root(Frost::FROST_COLLISION_MANAGER_BVHTREE);

	// We need to keep a registry of the named objects we are using - otherwise,
	//  we (currently) have no way of accessing them.
	std::vector<std::string> objNames;

	// Add objects to the scene...
	root.addObject(std::make_shared<Frost::BasicPhysicsObject>(10.f, Frost::MathConstants::MATRIX_IDENTITY,
		0.f, 0.f, Frost::Vect3(0.f, 150.f, 400.f)),
		"Ball");
	objNames.push_back("Ball");
	root.addObject(std::make_shared<Frost::BasicPhysicsObject>(0.f, Frost::MathConstants::MATRIX_ZERO,
		1.f, 1.f, Frost::Vect3(0.f, 0.f, 400.f)),
		"Table");
	objNames.push_back("Table");

	// Add collision information to the objects in our scene
	root["Ball"]->getCollidableData()->addCollisionObject(
		std::make_shared<Frost::CollisionSphere>(10.f, root["Ball"]->getPhysicsObject()->getPos(), root["Ball"]->getPhysicsObject()),
		Frost::MathConstants::MATRIX_IDENTITY);
	root["Table"]->getCollidableData()->addCollisionObject(
		std::make_shared<Frost::CollisionBox>(Frost::Vect3(130.f, 10.f, 130.f), root["Table"]->getPhysicsObject()->getPos(),
		Frost::MathConstants::QUATERNION_UNIT, root["Table"]->getPhysicsObject()),
		Frost::MathConstants::MATRIX_IDENTITY);

	// Add forces to the objects in our scene
	root.addForce(std::make_shared<Frost::GravityForce>(Frost::MathConstants::VECTOR_UNIT_Y * -1.f, 98.1f), "Ball");

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
		root.update(timer.getFrameTime());

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
